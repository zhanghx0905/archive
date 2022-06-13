#ifndef RECOVERY_MANAGER_H_
#define RECOVERY_MANAGER_H_

#include <unordered_map>

#include "defines.h"
#include "macros.h"
#include "manager/table_manager.h"

namespace thdb {

class RecoveryManager {
 public:
  RecoveryManager(TableManager* pTManager);
  ~RecoveryManager();

  void Redo();
  void Undo();
  void Log(const String& tableName, const PageSlotID& data);
  void ClearLog(const String& tableName);

 private:
  TableManager* _pTManager;
  std::unordered_map<String, std::vector<PageSlotID>> _wRecords;
};

}  // namespace thdb

#endif  // RECOVERY_MANAGER_H_
