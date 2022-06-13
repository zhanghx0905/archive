#include "page/node_page.h"

#include <assert.h>
#include <float.h>

#include "exception/exceptions.h"
#include "field/fields.h"
#include "macros.h"
#include "minios/os.h"

namespace thdb {

const PageOffset LEAF_OFFSET = 8;
const PageOffset USED_SLOT_OFFSET = 12;
const PageOffset KEY_TYPE_OFFSET = 16;
constexpr Size PAGESLOTID_SIZE = sizeof(PageSlotID);
using u8 = uint8_t;

NodePage::NodePage(FieldType iKeyType, bool bLeaf)
    : _iKeyType(iKeyType), _bLeaf(bLeaf) {
  // DONE: 基于自己实现的Store算法确定最大容量
  _nKeyLen = (iKeyType == FieldType::INT_TYPE) ? 4 : 8;
  _nCap = (PAGE_SIZE - HEADER_SIZE) / (_nKeyLen + PAGESLOTID_SIZE);
}

NodePage::NodePage(FieldType iKeyType, bool bLeaf,
                   const std::vector<Field *> &iKeyVec,
                   const std::vector<PageSlotID> &iChildVec)
    : _iKeyType(iKeyType),
      _bLeaf(bLeaf),
      _iKeyVec(std::move(iKeyVec)),
      _iChildVec(std::move(iChildVec)) {
  // DONE: 基于自己实现的Store算法确定最大容量
  _nKeyLen = (iKeyType == FieldType::INT_TYPE) ? 4 : 8;
  _nCap = (PAGE_SIZE - HEADER_SIZE) / (_nKeyLen + PAGESLOTID_SIZE);
}

NodePage::NodePage(PageID nPageID) : Page(nPageID) {
  // DONE: 从格式化页面中导入结点信息
  GetHeader((u8 *)&_bLeaf, 1, LEAF_OFFSET);
  GetHeader((u8 *)&_iKeyType, 4, KEY_TYPE_OFFSET);
  _nKeyLen = (_iKeyType == FieldType::INT_TYPE) ? 4 : 8;
  _nCap = (PAGE_SIZE - HEADER_SIZE) / (_nKeyLen + PAGESLOTID_SIZE);
  Size used_slot;
  GetHeader((u8 *)&used_slot, 4, USED_SLOT_OFFSET);

  PageOffset offset = 0;
  auto data(make_unique<u8[]>(_nKeyLen));
  for (Size i = 0; i < used_slot; ++i) {
    Field *key = (_iKeyType == FieldType::INT_TYPE) ? (Field *)new IntField()
                                                    : (Field *)new FloatField();
    GetData(data.get(), _nKeyLen, offset);
    key->SetData(data.get(), _nKeyLen);
    offset += _nKeyLen;
    PageSlotID value;
    GetData((u8 *)&value, PAGESLOTID_SIZE, offset);
    offset += PAGESLOTID_SIZE;
    _iKeyVec.emplace_back(key);
    _iChildVec.emplace_back(value);
  }
}

NodePage::~NodePage() {
  // DONE: 将结点信息格式化并写回到页面中
  SetHeader((u8 *)&_bLeaf, 1, LEAF_OFFSET);
  SetHeader((u8 *)&_iKeyType, 4, KEY_TYPE_OFFSET);
  Size used_slot = _iKeyVec.size();
  SetHeader((u8 *)&used_slot, 4, USED_SLOT_OFFSET);
  PageOffset offset = 0;
  auto data(make_unique<u8[]>(_nKeyLen));
  for (Size i = 0; i < used_slot; ++i) {
    _iKeyVec[i]->GetData(data.get(), _nKeyLen);
    SetData(data.get(), _nKeyLen, offset);
    offset += _nKeyLen;
    SetData((u8 *)&_iChildVec[i], PAGESLOTID_SIZE, offset);
    offset += PAGESLOTID_SIZE;
    delete _iKeyVec[i];
  }
}

#define MINUS1(v) ((v) ? (v - 1) : 0)
void NodePage::Insert(Field *pKey, const PageSlotID &iPair) {
  // DONE: 需要基于结点类型判断执行过程
  // 叶结点：
  // 1.确定插入位置后插入数据即可
  // 中间结点:
  // 1.确定执行插入函数的子结点
  // 2.对应的子结点执行插入函数
  // 3.判断子结点是否为满结点，满结点时执行分裂
  // 4.子结点分裂情况下需要更新KeyVec和ChildVec
  Size index = LowerBound(pKey);
  if (_bLeaf) {
    _iKeyVec.insert(_iKeyVec.begin() + index, pKey->Copy());
    _iChildVec.insert(_iChildVec.begin() + index, iPair);
  } else {
    index = MINUS1(index);
    auto page(make_unique<NodePage>(_iChildVec[index].first));
    page->Insert(pKey, iPair);
    _iKeyVec[index] = page->FirstKey();
    if (page->Full()) {
      auto newBroPage = page->Split();
      _iKeyVec.insert(_iKeyVec.begin() + index + 1, newBroPage->FirstKey());
      _iChildVec.insert(_iChildVec.begin() + index + 1,
                        {newBroPage->GetPageID(), 0});
    }
  }
  // ALERT:
  // 中间结点执行插入过程中，需要考虑到实际中间结点为空结点的特殊情况进行特判
  // ALERT: 对于头结点的插入可能更新头结点的Key值
  // ALERT: KeyVec中的Key的赋值需要使用深拷贝，否则会出现析构导致的问题
  // ALERT: 上层保证每次插入的iPair不同
}

Size NodePage::Delete(Field *pKey) {
  // DONE: 需要基于结点类型判断执行过程
  // 叶结点：
  // 1.确定删除位置后删除数据即可
  // 中间结点:
  // 1.确定执行删除函数的子结点
  // 2.对应的子结点执行删除函数
  // 3.判断子结点是否为满结点，空结点时清除空结点
  // 4.删除空结点情况下需要更新KeyVec和ChildVec
  Size start = LowerBound(pKey), end = UpperBound(pKey);
  Size cnt = 0;
  if (_bLeaf) {
    _iKeyVec.erase(_iKeyVec.begin() + start, _iKeyVec.begin() + end);
    _iChildVec.erase(_iChildVec.begin() + start, _iChildVec.begin() + end);
    cnt += end - start;
  } else {
    start = MINUS1(start);
    for (Size i = start; i < end; ++i) {
      auto page(make_unique<NodePage>(_iChildVec[i].first));
      cnt += page->Delete(pKey);
    }
  }
  return cnt;
  // ALERT: 注意删除结点过程中如果清除了Key则需要析构
  // ALERT:
  // 注意存在键值相同的情况发生，所以需要保证所有需要执行删除函数的子结点都执行了删除函数
  // ALERT: 可以适当简化合并函数，例如不删除空的中间结点
}

bool NodePage::Delete(Field *pKey, const PageSlotID &iPair) {
  // DONE: 需要基于结点类型判断执行过程
  // 叶结点：
  // 1.确定删除位置后删除数据即可
  // 中间结点:
  // 1.确定执行删除函数的子结点
  // 2.对应的子结点执行删除函数
  // 3.判断子结点是否为满结点，空结点时清除空结点
  // 4.删除空结点情况下需要更新KeyVec和ChildVec
  Size start = LowerBound(pKey), end = UpperBound(pKey);
  if (_bLeaf) {
    for (Size i = start; i < end; ++i) {
      if (_iChildVec[i] == iPair) {
        _iKeyVec.erase(_iKeyVec.begin() + i);
        _iChildVec.erase(_iChildVec.begin() + i);
        return true;
      }
    }
  } else {
    start = MINUS1(start);
    for (Size i = start; i < end; ++i) {
      auto page(make_unique<NodePage>(_iChildVec[i].first));
      if (page->Delete(pKey, iPair)) {
        if (page->Empty()) {
          _iKeyVec.erase(_iKeyVec.begin() + i);
          _iChildVec.erase(_iChildVec.begin() + i);
        } else {
          _iKeyVec[i] = page->FirstKey();
        }
        return true;
      }
    }
  }
  return false;
  // ALERT:
  // 由于Insert过程中保证了没用相同的Value值，所以只要成功删除一个结点即可保证删除成功
}

bool NodePage::Update(Field *pKey, const PageSlotID &iOld,
                      const PageSlotID &iNew) {
  // DONE: 需要基于结点类型判断执行过程
  // 叶结点：
  // 1.确定更新位置后更新数据即可
  // 中间结点:
  // 1.确定执行更新函数的子结点
  // 2.对应的子结点执行更新函数
  Size start = LowerBound(pKey), end = UpperBound(pKey);
  if (_bLeaf) {
    for (Size i = start; i < end; ++i) {
      if (_iChildVec[i] == iOld) {
        _iChildVec[i] == iNew;
        return true;
      }
    }
  } else {
    start = MINUS1(start);
    for (Size i = start; i < end; ++i) {
      if (make_unique<NodePage>(_iChildVec[i].first)->Update(pKey, iOld, iNew))
        return true;
    }
  }
  return false;
  // ALERT: 由于更新函数不改变结点内存储的容量，所以不需要结构变化
  // ALERT:
  // 由于Insert过程中保证了没用相同的Value值，所以只要成功更新一个结点即可保证更新成功
}

void NodePage::Range(Field *pLow, Field *pHigh, std::vector<PageSlotID> &res) {
  // DONE: 需要基于结点类型判断执行过程
  // 叶结点：
  // 1.确定上下界范围，返回这一区间内的所有Value值
  // 中间结点:
  // 1.确定所有可能包含上下界范围的子结点
  // 2.依次对添加各个子结点执行查询函数所得的结果
  Size start = LowerBound(pLow), end = LowerBound(pHigh);
  if (_bLeaf) {
    for (Size i = start; i < end; ++i) {
      res.emplace_back(_iChildVec[i]);
    }
  } else {
    start = MINUS1(start);
    for (Size i = start; i < end; ++i) {
      make_unique<NodePage>(_iChildVec[i].first)->Range(pLow, pHigh, res);
    }
  }
  // ALERT: 注意叶结点可能为空结点，需要针对这种情况进行特判
}

std::unique_ptr<NodePage> NodePage::Split() {
  std::vector<Field *> iKeyVec{};
  std::vector<PageSlotID> iChildVec{};
  Size mid = _nCap / 2;
  for (auto it = _iKeyVec.begin() + mid; it != _iKeyVec.end();) {
    iKeyVec.push_back(*it);
    it = _iKeyVec.erase(it);
  }
  for (auto it = _iChildVec.begin() + mid; it != _iChildVec.end();) {
    iChildVec.push_back(*it);
    it = _iChildVec.erase(it);
  }
  return make_unique<NodePage>(_iKeyType, _bLeaf, iKeyVec, iChildVec);
}

bool NodePage::Full() const { return _iKeyVec.size() == _nCap; }
bool NodePage::Empty() const { return _iKeyVec.empty(); }

FieldType NodePage::GetType() const { return _iKeyType; }

Field *NodePage::FirstKey() const {
  if (Empty()) return nullptr;
  return _iKeyVec[0]->Copy();
}

Size NodePage::LowerBound(Field *pField) {
  Size nBegin = 0, nEnd = _iKeyVec.size();
  while (nBegin < nEnd) {
    Size nMid = (nBegin + nEnd) / 2;
    if (!Less(_iKeyVec[nMid], pField, _iKeyType)) {
      nEnd = nMid;
    } else {
      nBegin = nMid + 1;
    }
  }
  return nBegin;
}

Size NodePage::UpperBound(Field *pField) {
  Size nBegin = 0, nEnd = _iKeyVec.size();
  while (nBegin < nEnd) {
    Size nMid = (nBegin + nEnd) / 2;
    if (Greater(_iKeyVec[nMid], pField, _iKeyType)) {
      nEnd = nMid;
    } else {
      nBegin = nMid + 1;
    }
  }
  return nBegin;
}

}  // namespace thdb
