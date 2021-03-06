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

  // Lab4 begin: ????????????????????? txn ???
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
  // DONE:?????????????????????????????????JOIN??????

  // ALERT:????????????????????????????????????????????????????????????JOIN?????????????????????????????????????????????????????????????????????
  // ALERT:????????????????????????JOIN?????????????????????????????????????????????
  // ALERT:??????????????????JOIN?????????????????????????????????????????????????????????????????????????????????????????????JOIN??????
  // ALERT:JOIN??????????????????Filter??????
  // ALERT:????????????????????????????????????NestedLoopJoin??????

  // TIPS:JoinCondition????????????JOIN????????????????????????
  // TIPS:??????GetTable(TableName)?????????????????????Table*?????????????????????lab1??????Table::GetRecord????????????Record*??????
  // TIPs:??????Table::GetColumnNames????????????Table???????????????????????????Record*????????????
  // TIPS:Record???????????????Copy,Sub,Add,Remove??????????????????????????????Record????????????
  // TIPS:??????GetColID/Type/Size?????????????????????????????????????????????????????????
  // TIPS:??????IsIndex?????????????????????????????????
  // TIPS:??????GetIndex??????????????????Index*??????

  // EXTRA:JOIN?????????????????????2?????????????????????????????????JOIN???????????????????????????????????????,????????????????????????????????????
  // EXTRA:?????????JOIN??????????????????????????????????????????????????????JOIN???????????????
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
