#include "record/fixed_record.h"

#include <assert.h>

#include "exception/exceptions.h"
#include "field/fields.h"

namespace thdb {

FixedRecord::FixedRecord(Size nFieldSize,
                         const std::vector<FieldType> &iTypeVec,
                         const std::vector<Size> &iSizeVec)
    : Record(nFieldSize), _iTypeVec(iTypeVec), _iSizeVec(iSizeVec) {
  assert(_iTypeVec.size() == nFieldSize);
  assert(_iSizeVec.size() == nFieldSize);
}

Size FixedRecord::Load(const uint8_t *src) {
  Clear();  // _iFields now is vector of nullptr
  // LAB1 BEGIN
  // DONE: 反序列化，为定长记录导入各个字段数据
  Size tot = 0;
  for (FieldID i = 0; i < _iFields.size(); ++i) {
    Field *newField;
    switch (_iTypeVec[i]) {
      case FieldType::FLOAT_TYPE:
        newField = new FloatField();
        break;
      case FieldType::INT_TYPE:
        newField = new IntField();
        break;
      case FieldType::STRING_TYPE:
        newField = new StringField(_iSizeVec[i]);
        break;
      default:
        newField = new NoneField();
    }
    newField->SetData(src + tot, _iSizeVec[i]);
    tot += _iSizeVec[i];
    _iFields[i] = newField;
  }
  return tot;
  // TIPS: 利用Field的抽象方法SetData导入数据
  // TIPS: 基于类型判断构建的指针类型
  // LAB1 END
}

Size FixedRecord::Store(uint8_t *dst) const {
  // LAB1 BEGIN
  // DONE: 序列化，将定长数据转化为特定格式
  Size tot = 0;
  for (FieldID i = 0; i < _iFields.size(); ++i) {
    _iFields[i]->GetData(dst + tot, _iSizeVec[i]);
    tot += _iSizeVec[i];
  }
  return tot;
  // TIPS: 利用Field的抽象方法GetData写出数据
  // TIPS: 基于类型进行dynamic_cast进行指针转化
  // LAB1 END
}

void FixedRecord::Build(const std::vector<String> &iRawVec) {
  assert(iRawVec.size() == _iTypeVec.size());
  Clear();
  for (FieldID i = 0; i < _iFields.size(); ++i) {
    FieldType iType = _iTypeVec[i];
    if (iRawVec[i] == "NULL") {
      SetField(i, new NoneField());
      continue;
    }
    if (iType == FieldType::INT_TYPE) {
      int nVal = std::stoi(iRawVec[i]);
      SetField(i, new IntField(nVal));
    } else if (iType == FieldType::FLOAT_TYPE) {
      double fVal = std::stod(iRawVec[i]);
      SetField(i, new FloatField(fVal));
    } else if (iType == FieldType::STRING_TYPE) {
      SetField(i, new StringField(iRawVec[i].substr(1, iRawVec[i].size() - 2)));
    } else {
      throw RecordTypeException();
    }
  }
}

Record *FixedRecord::Copy() const {
  Record *pRecord = new FixedRecord(GetSize(), _iTypeVec, _iSizeVec);
  for (Size i = 0; i < GetSize(); ++i)
    pRecord->SetField(i, GetField(i)->Copy());
  return pRecord;
}

void FixedRecord::Sub(const std::vector<Size> &iPos) {
  bool bInSub[GetSize()];
  memset(bInSub, 0, GetSize() * sizeof(bool));
  for (const auto nPos : iPos) bInSub[nPos] = 1;
  auto itField = _iFields.begin();
  auto itType = _iTypeVec.begin();
  auto itSize = _iSizeVec.begin();
  auto nSize = GetSize();
  for (Size i = 0; i < nSize; ++i) {
    if (!bInSub[i]) {
      Field *pField = *itField;
      if (pField) delete pField;
      itField = _iFields.erase(itField);
      itType = _iTypeVec.erase(itType);
      itSize = _iSizeVec.erase(itSize);
    } else {
      ++itField;
      ++itType;
      ++itSize;
    }
  }
}

void FixedRecord::Add(Record *pRecord) {
  FixedRecord *pFixed = dynamic_cast<FixedRecord *>(pRecord);
  assert(pFixed != nullptr);
  for (Size i = 0; i < pFixed->GetSize(); ++i) {
    _iFields.push_back(pFixed->GetField(i)->Copy());
    _iTypeVec.push_back(pFixed->_iTypeVec[i]);
    _iSizeVec.push_back(pFixed->_iSizeVec[i]);
  }
}

void FixedRecord::Remove(FieldID nPos) {
  Record::Remove(nPos);
  auto itType = _iTypeVec.begin() + nPos;
  auto itSize = _iSizeVec.begin() + nPos;
  _iTypeVec.erase(itType);
  _iSizeVec.erase(itSize);
}

}  // namespace thdb
