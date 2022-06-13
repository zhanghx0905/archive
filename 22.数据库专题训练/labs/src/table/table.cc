#include "table/table.h"

#include <assert.h>

#include <algorithm>

#include "macros.h"
#include "minios/os.h"
#include "page/record_page.h"
#include "record/fixed_record.h"

namespace thdb {

PageID NextPageID(PageID nCur) {
  LinkedPage *pPage = new LinkedPage(nCur);
  PageID nNext = pPage->GetNextID();
  delete pPage;
  return nNext;
}

Table::Table(PageID nTableID) {
  pTable = new TablePage(nTableID);

  _nHeadID = pTable->GetHeadID();
  _nTailID = pTable->GetTailID();
  // init not full page set
  auto pageIter = _nHeadID;
  while (pageIter != NULL_PAGE) {
    auto pPage = make_unique<RecordPage>(pageIter);
    if (!pPage->Full()) {
      _pageSet.insert(pageIter);
    }
    pageIter = pPage->GetNextID();
  }
  _nNotFull = *_pageSet.begin();
}

Table::~Table() { delete pTable; }

Record *Table::GetRecord(PageID nPageID, SlotID nSlotID) {
  // LAB1 BEGIN
  // DONE: 获得一条记录
  auto pRecord = dynamic_cast<FixedRecord *>(EmptyRecord());
  uint8_t *src = make_unique<RecordPage>(nPageID)->GetRecord(nSlotID);
  pRecord->Load(src);
  delete[] src;
  return pRecord;
  // TIPS: 利用RecordPage::GetRecord获取无格式记录数据
  // TIPS: 利用TablePage::GetFieldSize, GetTyepVec,
  // GetSizeVec三个函数可以构建空的FixedRecord对象 TIPS:
  // 利用Record::Load导入数据 ALERT: 需要注意析构所有不会返回的内容
  // LAB1 END
}

PageSlotID Table::InsertRecord(Record *pRecord) {
  // LAB1 BEGIN
  // DONE: 插入一条记录
  auto pageId = _nNotFull;
  // 控制析构
  auto pPage = make_unique<RecordPage>(_nNotFull);
  auto dst = make_unique<uint8_t[]>(pTable->GetTotalSize());
  pRecord->Store(dst.get());
  SlotID slotID = pPage->InsertRecord(dst.get());
  pPage.reset();
  UpdateNotFull(_nNotFull);
  return PageSlotID(pageId, slotID);
  // TIPS: 利用_nNotFull来获取有空间的页面
  // TIPS: 利用Record::Store获得序列化数据
  // TIPS: 利用RecordPage::InsertRecord插入数据
  // TIPS: 注意页满时更新_nNotFull
  // LAB1 END
}

void Table::DeleteRecord(PageID nPageID, SlotID nSlotID) {
  // LAB1 BEGIN
  // DONE: 删除一条记录
  make_unique<RecordPage>(nPageID)->DeleteRecord(nSlotID);
  UpdateNotFull(nPageID);
  // TIPS: 利用RecordPage::DeleteRecord插入数据
  // TIPS: 注意更新_nNotFull来保证较高的页面空间利用效率
  // LAB1 END
}

void Table::UpdateRecord(PageID nPageID, SlotID nSlotID,
                         const std::vector<Transform> &iTrans) {
  // LAB1 BEGIN
  // DONE: 更新一条记录
  auto pPage = make_unique<RecordPage>(nPageID);
  uint8_t *src = pPage->GetRecord(nSlotID);
  auto pRecord = dynamic_cast<FixedRecord *>(EmptyRecord());

  pRecord->Load(src);
  for (const auto &trans : iTrans) {
    pRecord->SetField(trans.GetPos(), trans.GetField());
  }
  pRecord->Store(src);
  pPage->UpdateRecord(nSlotID, src);

  delete[] src;
  delete pRecord;
  // TIPS: 将新的记录序列化
  // TIPS: 利用RecordPage::UpdateRecord更新一条数据
  // LAB1 END
}

std::vector<PageSlotID> Table::SearchRecord(Condition *pCond) {
  // LAB1 BEGIN
  // DONE: 对记录的条件检索
  // pCond = nullptr 表示查找该表的所有记录
  std::vector<PageSlotID> res;
  auto pRecord = dynamic_cast<FixedRecord *>(EmptyRecord());
  PageID pageIter = _nHeadID;
  while (pageIter != NULL_PAGE) {
    auto pPage = make_unique<RecordPage>(pageIter);
    Size capacity = pPage->GetCap();
    for (SlotID i = 0; i < capacity; ++i) {
      if (!pPage->HasRecord(i)) {  // The slot doesn't have valid record
        continue;
      }
      uint8_t *src = pPage->GetRecord(i);
      pRecord->Load(src);
      if (pCond == nullptr || pCond->Match(*pRecord)) {
        res.emplace_back(pageIter, i);
      }
      delete[] src;
    }
    pageIter = pPage->GetNextID();
  }
  delete pRecord;
  return res;
  // TIPS: 仿照InsertRecord从无格式数据导入原始记录
  // TIPS: 依次导入各条记录进行条件判断
  // TIPS: Condition的抽象方法Match可以判断Record是否满足检索条件
  // TIPS: 返回所有符合条件的结果的pair<PageID,SlotID>
  // LAB1 END
}

void Table::SearchRecord(std::vector<PageSlotID> &iPairs, Condition *pCond) {
  if (!pCond) return;
  auto it = iPairs.begin();
  while (it != iPairs.end()) {
    Record *pRecord = GetRecord(it->first, it->second);
    if (!pCond->Match(*pRecord)) {
      it = iPairs.erase(it);
    } else
      ++it;
    delete pRecord;
  }
}

void Table::Clear() {
  PageID nBegin = _nHeadID;
  while (nBegin != NULL_PAGE) {
    PageID nTemp = nBegin;
    nBegin = NextPageID(nBegin);
    MiniOS::GetOS()->DeletePage(nTemp);
  }
}

void Table::UpdateNotFull(PageID nPageID) {
  // LAB1 BEGIN
  // DONE: 实现一个快速查找非满记录页面的算法
  if (make_unique<RecordPage>(nPageID)->Full()) {
    _pageSet.erase(nPageID);
  } else {
    _pageSet.insert(nPageID);
  }
  if (_pageSet.empty()) {
    auto pLastPage = make_unique<RecordPage>(_nTailID);
    auto pNewPage = make_unique<RecordPage>(pTable->GetTotalSize(), false);
    pLastPage->PushBack(pNewPage.get());
    _nTailID = pNewPage->GetPageID();
    _pageSet.insert(_nTailID);
  }
  _nNotFull = *_pageSet.begin();
  // ALERT: ！！！一定要注意！！！
  // 不要同时建立两个指向相同磁盘位置的且可变对象，否则会出现一致性问题
  // ALERT: 可以适当增加传入参数，本接口不会被外部函数调用，例如额外传入Page
  // *指针
  // TIPS:
  // 充分利用链表性质，注意全满时需要在结尾_pTable->GetTailID对应结点后插入新的结点，并更新_pTable的TailID
  // TIPS: 只需要保证均摊复杂度较低即可
  // LAB1 END
}

FieldID Table::GetPos(const String &sCol) const { return pTable->GetPos(sCol); }

FieldType Table::GetType(const String &sCol) const {
  return pTable->GetType(sCol);
}

Size Table::GetSize(const String &sCol) const { return pTable->GetSize(sCol); }

Record *Table::EmptyRecord() const {
  FixedRecord *pRecord = new FixedRecord(
      pTable->GetFieldSize(), pTable->GetTypeVec(), pTable->GetSizeVec());
  return pRecord;
}

bool CmpByFieldID(const std::pair<String, FieldID> &a,
                  const std::pair<String, FieldID> &b) {
  return a.second < b.second;
}

std::vector<String> Table::GetColumnNames() const {
  std::vector<String> iVec{};
  std::vector<std::pair<String, FieldID>> iPairVec(pTable->_iColMap.begin(),
                                                   pTable->_iColMap.end());
  std::sort(iPairVec.begin(), iPairVec.end(), CmpByFieldID);
  for (const auto &it : iPairVec) iVec.push_back(it.first);
  return iVec;
}

}  // namespace thdb
