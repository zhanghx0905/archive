#include "manager/recovery_manager.h"

#include "exception/exceptions.h"
#include "field/fields.h"
#include "minios/os.h"
#include "page/record_page.h"
#include "record/fixed_record.h"

namespace thdb {

RecoveryManager::RecoveryManager(TableManager* pTManager)
    : _pTManager(pTManager) {
  _wRecords = {};
  auto tableNames = _pTManager->GetTableNames();
  for (const auto& name : tableNames) {
    auto pPage(make_unique<RecordPage>(_pTManager->GetLogPageID(name)));
    for (Size i = 0; i < pPage->GetCap(); ++i) {
      if (!pPage->HasRecord(i)) break;
      uint32_t* pData = (uint32_t*)pPage->GetRecord(i);
      _wRecords[name].emplace_back(PageSlotID{pData[0], pData[1]});
    }
  }
}

RecoveryManager::~RecoveryManager() {
  // 必须保证 pTManager 此时还未被析构
  for (auto& iter : _wRecords) {
    PageID id;
    try {
      id = _pTManager->GetLogPageID(iter.first);
    } catch (TableNotExistException e) {
      continue;
    }
    auto pPage(make_unique<RecordPage>(id));
    pPage->Clear();
    uint32_t pData[2];
    for (auto& item : iter.second) {
      pData[0] = item.first, pData[1] = item.second;
      pPage->InsertRecord((uint8_t*)pData);
    }
  }
}

void RecoveryManager::Redo() {
  // force, 不必实现 Redo
}

void RecoveryManager::Undo() {
  for (auto& iter : _wRecords) {
    Table* pTable = _pTManager->GetTable(iter.first);
    for (auto& item : iter.second) {
      pTable->DeleteRecord(item.first, item.second);
    }
  }
  _wRecords.clear();
}

void RecoveryManager::Log(const String& tableName, const PageSlotID& data) {
  _wRecords[tableName].emplace_back(data);
}

void RecoveryManager::ClearLog(const String& tableName) {
  _wRecords[tableName].clear();
}
}  // namespace thdb
