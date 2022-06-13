#include "system/instance.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "condition/conditions.h"
#include "exception/exceptions.h"
#include "manager/table_manager.h"
#include "record/fixed_record.h"

using std::vector;
namespace thdb {

const String TXN_BEG_COLNAME = "txnBegID";
const String TXN_END_COLNAME = "txnEndID";

Instance::Instance() {
  _pTableManager = new TableManager();
  _pIndexManager = new IndexManager();
  _pRecoveryManager = new RecoveryManager(_pTableManager);
  _pTransactionManager = new TransactionManager(_pRecoveryManager);
}

Instance::~Instance() {
  delete _pTransactionManager;
  delete _pRecoveryManager;
  delete _pTableManager;
  delete _pIndexManager;
}

Table *Instance::GetTable(const String &sTableName) const {
  return _pTableManager->GetTable(sTableName);
}

bool Instance::CreateTable(const String &sTableName, const Schema &iSchema,
                           bool useTxn) {
  // Lab4 begin
  if (useTxn) {
    vector<Column> cols{};
    for (Size i = 0; i < iSchema.GetSize(); ++i) {
      cols.emplace_back(iSchema.GetColumn(i));
    }
    cols.insert(cols.end(), {Column(TXN_BEG_COLNAME, FieldType::INT_TYPE),
                             Column(TXN_END_COLNAME, FieldType::INT_TYPE)});
    _pTableManager->AddTable(sTableName, Schema(cols));
    // Lab4 end
  } else {
    _pTableManager->AddTable(sTableName, iSchema);
  }
  return true;
}

bool Instance::DropTable(const String &sTableName) {
  for (const auto &sColName : _pIndexManager->GetTableIndexes(sTableName))
    _pIndexManager->DropIndex(sTableName, sColName);
  _pTableManager->DropTable(sTableName);
  return true;
}

FieldID Instance::GetColID(const String &sTableName,
                           const String &sColName) const {
  Table *pTable = GetTable(sTableName);
  if (pTable == nullptr) throw TableException();
  return pTable->GetPos(sColName);
}

FieldType Instance::GetColType(const String &sTableName,
                               const String &sColName) const {
  Table *pTable = GetTable(sTableName);
  if (pTable == nullptr) throw TableException();
  return pTable->GetType(sColName);
}

Size Instance::GetColSize(const String &sTableName,
                          const String &sColName) const {
  Table *pTable = GetTable(sTableName);
  if (pTable == nullptr) throw TableException();
  return pTable->GetSize(sColName);
}

bool CmpPageSlotID(const PageSlotID &iA, const PageSlotID &iB) {
  if (iA.first == iB.first) return iA.second < iB.second;
  return iA.first < iB.first;
}

std::vector<PageSlotID> Intersection(std::vector<PageSlotID> iA,
                                     std::vector<PageSlotID> iB) {
  std::sort(iA.begin(), iA.end(), CmpPageSlotID);
  std::sort(iB.begin(), iB.end(), CmpPageSlotID);
  std::vector<PageSlotID> iRes{};
  std::set_intersection(iA.begin(), iA.end(), iB.begin(), iB.end(),
                        std::back_inserter(iRes));
  return iRes;
}

std::vector<PageSlotID> Instance::Search(
    const String &sTableName, Condition *pCond,
    const std::vector<Condition *> &iIndexCond, Transaction *txn) {
  Table *pTable = GetTable(sTableName);
  if (pTable == nullptr) throw TableException();
  if (iIndexCond.size() > 0) {
    IndexCondition *pIndexCond = dynamic_cast<IndexCondition *>(iIndexCond[0]);
    assert(pIndexCond != nullptr);
    auto iName = pIndexCond->GetIndexName();
    auto iRange = pIndexCond->GetIndexRange();
    std::vector<PageSlotID> iRes =
        GetIndex(iName.first, iName.second)->Range(iRange.first, iRange.second);
    for (Size i = 1; i < iIndexCond.size(); ++i) {
      IndexCondition *pIndexCond =
          dynamic_cast<IndexCondition *>(iIndexCond[i]);
      auto iName = pIndexCond->GetIndexName();
      auto iRange = pIndexCond->GetIndexRange();
      iRes = Intersection(iRes, GetIndex(iName.first, iName.second)
                                    ->Range(iRange.first, iRange.second));
    }
    return iRes;
  } else {
    std::vector<PageSlotID> iRes = pTable->SearchRecord(pCond);
    // Lab4 begin
    if (txn) {
      FieldID txnBegPos = pTable->GetPos(TXN_BEG_COLNAME);
      auto it = iRes.begin();
      while (it != iRes.end()) {
        Record *pRecord = pTable->GetRecord(it->first, it->second);
        TxnID txnBeg = ((IntField *)pRecord->GetField(txnBegPos))->GetIntData(),
              txnEnd =
                  ((IntField *)pRecord->GetField(txnBegPos + 1))->GetIntData();
        if (txn->CanSee(txnBeg) && !txn->CanSee(txnEnd)) {
          it++;
        } else {
          it = iRes.erase(it);
        }
        delete pRecord;
      }
    }
    // Lab4 end
    return iRes;
  }
}

PageSlotID Instance::Insert(const String &sTableName,
                            const std::vector<String> &iRawVec,
                            Transaction *txn) {
  Table *pTable = GetTable(sTableName);
  if (pTable == nullptr) throw TableException();
  Record *pRecord = pTable->EmptyRecord();

  // Lab4 begin: 插入记录时增加 txn 列
  if (txn) {
    std::vector<String> rawVec = std::move(iRawVec);
    rawVec.insert(rawVec.end(),
                  {std::to_string(txn->GetID()), std::to_string(INT32_MAX)});
    pRecord->Build(rawVec);
  } else {
    pRecord->Build(iRawVec);
  }
  PageSlotID iPair = pTable->InsertRecord(pRecord);
  if (txn) {
    txn->Log(sTableName, iPair);
  }
  // Lab4 end
  // Handle Insert on Index
  if (_pIndexManager->HasIndex(sTableName)) {
    auto iColNames = _pIndexManager->GetTableIndexes(sTableName);
    for (const auto &sCol : iColNames) {
      FieldID nPos = pTable->GetPos(sCol);
      Field *pKey = pRecord->GetField(nPos);
      _pIndexManager->GetIndex(sTableName, sCol)->Insert(pKey, iPair);
    }
  }

  delete pRecord;
  return iPair;
}

uint32_t Instance::Delete(const String &sTableName, Condition *pCond,
                          const std::vector<Condition *> &iIndexCond,
                          Transaction *txn) {
  auto iResVec = Search(sTableName, pCond, iIndexCond);
  Table *pTable = GetTable(sTableName);
  bool bHasIndex = _pIndexManager->HasIndex(sTableName);
  // Lab4 beg
  FieldID txnEndPos;
  if (txn) txnEndPos = pTable->GetPos(TXN_END_COLNAME);
  // Lab4 end
  for (const auto &iPair : iResVec) {
    // Handle Delete on Index
    if (bHasIndex) {
      Record *pRecord = pTable->GetRecord(iPair.first, iPair.second);
      auto iColNames = _pIndexManager->GetTableIndexes(sTableName);
      for (const auto &sCol : iColNames) {
        FieldID nPos = pTable->GetPos(sCol);
        Field *pKey = pRecord->GetField(nPos);
        _pIndexManager->GetIndex(sTableName, sCol)->Delete(pKey, iPair);
      }
      delete pRecord;
    }
    // Lab4 beg
    if (txn) {
      pTable->UpdateRecord(iPair.first, iPair.second,
                           {Transform(txnEndPos, FieldType::INT_TYPE,
                                      std::to_string(txn->GetID()))});
    } else {
      pTable->DeleteRecord(iPair.first, iPair.second);
    }
    // Lab4 end
  }
  return iResVec.size();
}

uint32_t Instance::Update(const String &sTableName, Condition *pCond,
                          const std::vector<Condition *> &iIndexCond,
                          const std::vector<Transform> &iTrans,
                          Transaction *txn) {
  auto iResVec = Search(sTableName, pCond, iIndexCond);
  Table *pTable = GetTable(sTableName);
  bool bHasIndex = _pIndexManager->HasIndex(sTableName);
  // Lab4 beg
  FieldID txnEndPos;
  if (txn) txnEndPos = pTable->GetPos(TXN_END_COLNAME);
  // Lab4 end
  for (const auto &iPair : iResVec) {
    // Handle Delete on Index
    if (bHasIndex) {
      Record *pRecord = pTable->GetRecord(iPair.first, iPair.second);
      auto iColNames = _pIndexManager->GetTableIndexes(sTableName);
      for (const auto &sCol : iColNames) {
        FieldID nPos = pTable->GetPos(sCol);
        Field *pKey = pRecord->GetField(nPos);
        _pIndexManager->GetIndex(sTableName, sCol)->Delete(pKey, iPair);
      }
      delete pRecord;
    }
    if (txn) {
      Record *pRecord = pTable->GetRecord(iPair.first, iPair.second);
      PageSlotID newRecID = pTable->InsertRecord(pRecord);

      std::vector<Transform> newTrans = std::move(iTrans);
      newTrans.emplace_back(Transform(txnEndPos - 1, FieldType::INT_TYPE,
                                      std::to_string(txn->GetID())));
      pTable->UpdateRecord(newRecID.first, newRecID.second, newTrans);
      pTable->UpdateRecord(iPair.first, iPair.second,
                           {Transform(txnEndPos, FieldType::INT_TYPE,
                                      std::to_string(txn->GetID()))});
      delete pRecord;
    } else {
      pTable->UpdateRecord(iPair.first, iPair.second, iTrans);
    }
    // Handle Delete on Index
    if (bHasIndex) {
      Record *pRecord = pTable->GetRecord(iPair.first, iPair.second);
      auto iColNames = _pIndexManager->GetTableIndexes(sTableName);
      for (const auto &sCol : iColNames) {
        FieldID nPos = pTable->GetPos(sCol);
        Field *pKey = pRecord->GetField(nPos);
        _pIndexManager->GetIndex(sTableName, sCol)->Insert(pKey, iPair);
      }
      delete pRecord;
    }
  }
  return iResVec.size();
}

Record *Instance::GetRecord(const String &sTableName, const PageSlotID &iPair,
                            Transaction *txn) const {
  Table *pTable = GetTable(sTableName);
  Record *record = pTable->GetRecord(iPair.first, iPair.second);
  // Lab4 begin
  if (txn) {
    FieldID txnEndPos = record->GetSize() - 1;
    record->Remove(txnEndPos);
    record->Remove(txnEndPos - 1);
  }
  // Lab4 end
  return record;
}

std::vector<Record *> Instance::GetTableInfos(const String &sTableName) const {
  std::vector<Record *> iVec{};
  for (const auto &sName : GetColumnNames(sTableName)) {
    FixedRecord *pDesc = new FixedRecord(
        3,
        {FieldType::STRING_TYPE, FieldType::STRING_TYPE, FieldType::INT_TYPE},
        {COLUMN_NAME_SIZE, 10, 4});
    pDesc->SetField(0, new StringField(sName));
    pDesc->SetField(1,
                    new StringField(toString(GetColType(sTableName, sName))));
    pDesc->SetField(2, new IntField(GetColSize(sTableName, sName)));
    iVec.push_back(pDesc);
  }
  return iVec;
}
std::vector<String> Instance::GetTableNames() const {
  return _pTableManager->GetTableNames();
}
std::vector<String> Instance::GetColumnNames(const String &sTableName) const {
  return _pTableManager->GetColumnNames(sTableName);
}

bool Instance::IsIndex(const String &sTableName, const String &sColName) const {
  return _pIndexManager->IsIndex(sTableName, sColName);
}

Index *Instance::GetIndex(const String &sTableName,
                          const String &sColName) const {
  return _pIndexManager->GetIndex(sTableName, sColName);
}

std::vector<Record *> Instance::GetIndexInfos() const {
  std::vector<Record *> iVec{};
  for (const auto &iPair : _pIndexManager->GetIndexInfos()) {
    FixedRecord *pInfo =
        new FixedRecord(4,
                        {FieldType::STRING_TYPE, FieldType::STRING_TYPE,
                         FieldType::STRING_TYPE, FieldType::INT_TYPE},
                        {TABLE_NAME_SIZE, COLUMN_NAME_SIZE, 10, 4});
    pInfo->SetField(0, new StringField(iPair.first));
    pInfo->SetField(1, new StringField(iPair.second));
    pInfo->SetField(
        2, new StringField(toString(GetColType(iPair.first, iPair.second))));
    pInfo->SetField(3, new IntField(GetColSize(iPair.first, iPair.second)));
    iVec.push_back(pInfo);
  }
  return iVec;
}

bool Instance::CreateIndex(const String &sTableName, const String &sColName,
                           FieldType iType) {
  auto iAll = Search(sTableName, nullptr, {});
  _pIndexManager->AddIndex(sTableName, sColName, iType);
  Table *pTable = GetTable(sTableName);
  // Handle Exists Data
  for (const auto &iPair : iAll) {
    FieldID nPos = pTable->GetPos(sColName);
    Record *pRecord = pTable->GetRecord(iPair.first, iPair.second);
    Field *pKey = pRecord->GetField(nPos);
    _pIndexManager->GetIndex(sTableName, sColName)->Insert(pKey, iPair);
    delete pRecord;
  }
  return true;
}

bool Instance::DropIndex(const String &sTableName, const String &sColName) {
  auto iAll = Search(sTableName, nullptr, {});
  Table *pTable = GetTable(sTableName);
  for (const auto &iPair : iAll) {
    FieldID nPos = pTable->GetPos(sColName);
    Record *pRecord = pTable->GetRecord(iPair.first, iPair.second);
    Field *pKey = pRecord->GetField(nPos);
    _pIndexManager->GetIndex(sTableName, sColName)->Delete(pKey, iPair);
    delete pRecord;
  }
  _pIndexManager->DropIndex(sTableName, sColName);
  return true;
}

std::pair<std::vector<String>, std::vector<Record *>> Instance::Join(
    std::map<String, std::vector<PageSlotID>> &iResultMap,
    std::vector<Condition *> &iJoinConds) {
  // LAB3 BEGIN
  // DONE:实现正确且高效的表之间JOIN过程

  // ALERT:由于实现临时表存储具有一定难度，所以允许JOIN过程中将中间结果保留在内存中，不需要存入临时表
  // ALERT:一定要注意，存在JOIN字段值相同的情况，需要特别重视
  // ALERT:针对于不同的JOIN情况（此处只需要考虑数据量和是否为索引列），可以选择使用不同的JOIN算法
  // ALERT:JOIN前已经经过了Filter过程
  // ALERT:建议不要使用不经过优化的NestedLoopJoin算法

  // TIPS:JoinCondition中保存了JOIN两方的表名和列名
  // TIPS:利用GetTable(TableName)的方式可以获得Table*指针，之后利用lab1中的Table::GetRecord获得初始Record*数据
  // TIPs:利用Table::GetColumnNames可以获得Table初始的列名，与初始Record*顺序一致
  // TIPS:Record对象添加了Copy,Sub,Add,Remove函数，方便同学们对于Record进行处理
  // TIPS:利用GetColID/Type/Size三个函数可以基于表名和列名获得列的信息
  // TIPS:利用IsIndex可以判断列是否存在索引
  // TIPS:利用GetIndex可以获得索引Index*指针

  // EXTRA:JOIN的表的数量超过2时，所以需要先计算一个JOIN执行计划（不要求复杂算法）,有兴趣的同学可以自行实现
  // EXTRA:在多表JOIN时，可以采用并查集或执行树来确定执行JOIN的数据内容
  vector<Record *> records;
  std::unordered_set<String> mergedTables;
  for (auto iter = iJoinConds.rbegin(); iter != iJoinConds.rend(); ++iter) {
    auto joinCond = dynamic_cast<JoinCondition *>(*iter);
    vector<Record *> recordsA, recordsB;
    Table *pTableA = GetTable(joinCond->sTableA),
          *pTableB = GetTable(joinCond->sTableB);
    FieldID PosA = pTableA->GetPos(joinCond->sColA),
            PosB = pTableB->GetPos(joinCond->sColB);
    FieldType fType = pTableA->GetType(joinCond->sColA);

    if (mergedTables.count(joinCond->sTableA)) {
      recordsA = std::move(records);
    } else {
      mergedTables.insert(joinCond->sTableA);
      for (auto &pageSlot : iResultMap[joinCond->sTableA]) {
        recordsA.emplace_back(
            pTableA->GetRecord(pageSlot.first, pageSlot.second));
      }
    }
    sort(recordsA.begin(), recordsA.end(),
         [&](const Record *rA, const Record *rB) {
           return Less(rA->GetField(PosA), rB->GetField(PosA), fType);
         });
    if (mergedTables.count(joinCond->sTableB)) {
      recordsB = std::move(records);
    } else {
      mergedTables.insert(joinCond->sTableB);
      for (auto &pageSlot : iResultMap[joinCond->sTableB]) {
        recordsB.emplace_back(
            pTableB->GetRecord(pageSlot.first, pageSlot.second));
      }
    }
    sort(recordsB.begin(), recordsB.end(),
         [&](const Record *rA, const Record *rB) {
           return Less(rA->GetField(PosB), rB->GetField(PosB), fType);
         });

    Size begin = 0;
    for (Record *recordA : recordsA) {
      bool flag = true;
      for (Size idx = begin; idx < recordsB.size(); ++idx) {
        Record *recordB = recordsB[idx];
        if (Equal(recordA->GetField(PosA), recordB->GetField(PosB), fType)) {
          Record *tmp = recordA->Copy();
          tmp->Add(recordB);
          records.emplace_back(tmp);
          if (flag) {
            begin = idx;
            flag = false;
          }
        } else if (Less(recordA->GetField(PosA), recordB->GetField(PosB),
                        fType)) {
          break;
        }
      }
    }
  }
  return {vector<String>(), records};
  // LAB3 END
}

}  // namespace thdb
