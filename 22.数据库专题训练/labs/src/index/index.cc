#include "index/index.h"

#include "page/node_page.h"

namespace thdb {
Index::Index(FieldType iType) {
  // DONE: 建立一个新的根结点
  _nRootID = make_unique<NodePage>(iType, true)->GetPageID();
}

Index::Index(PageID nPageID) {
  // DONE: 记录RootID即可
  _nRootID = nPageID;
}

Index::~Index() {
  // DONE: 如果不添加额外的指针，理论上不用额外写回内容
}

void Index::Clear() {
  // DONE: 通过测试不必实现
}

PageID Index::GetRootID() const { return _nRootID; }

bool Index::Insert(Field *pKey, const PageSlotID &iPair) {
  // DONE: 利用RootID获得根结点
  // DONE: 利用根结点的Insert执行插入
  // DONE: 根结点满时，需要进行分裂操作，同时更新RootID
  auto page(make_unique<NodePage>(_nRootID));
  page->Insert(pKey, iPair);
  if (page->Full()) {
    auto newPage = page->Split();
    FieldType type = newPage->GetType();
    std::vector<Field *> keyVec{page->FirstKey(), newPage->FirstKey()};
    std::vector<PageSlotID> childVec{{page->GetPageID(), 0},
                                     {newPage->GetPageID(), 0}};
    auto newRootPage = make_unique<NodePage>(type, false, keyVec, childVec);
    _nRootID = newRootPage->GetPageID();
  }
  return true;
}

Size Index::Delete(Field *pKey) {
  // ALERT:
  // 结点合并实现难度较高，没有测例，不要求实现，感兴趣的同学可自行实现并设计测例
  return make_unique<NodePage>(_nRootID)->Delete(pKey);
}

bool Index::Delete(Field *pKey, const PageSlotID &iPair) {
  // ALERT:
  // 结点合并实现难度较高，没有测例，不要求实现，感兴趣的同学可自行实现并设计测例
  // DONE: 利用RootID获得根结点
  // DONE: 利用根结点的Delete执行删除
  return make_unique<NodePage>(_nRootID)->Delete(pKey, iPair);
}

bool Index::Update(Field *pKey, const PageSlotID &iOld,
                   const PageSlotID &iNew) {
  // DONE: 利用RootID获得根结点
  // DONE: 利用根结点的Update执行删除
  return make_unique<NodePage>(_nRootID)->Update(pKey, iOld, iNew);
}

std::vector<PageSlotID> Index::Range(Field *pLow, Field *pHigh) {
  // DONE: 利用RootID获得根结点
  // DONE: 利用根结点的Range执行范围查找
  std::vector<PageSlotID> res{};
  make_unique<NodePage>(_nRootID)->Range(pLow, pHigh, res);
  return res;
}

}  // namespace thdb
