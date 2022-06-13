#include "transaction/transaction.h"

#include "manager/recovery_manager.h"

namespace thdb {

Transaction::Transaction(TxnID txnID,
                         const std::unordered_set<TxnID>& activeTxSet,
                         RecoveryManager* pRManager)
    : _txnID(txnID), _activeTxSet(activeTxSet), _pRManager(pRManager) {}

bool Transaction::CanSee(TxnID txnID) const {
  return _txnID == txnID || (txnID < _txnID && _activeTxSet.count(txnID) == 0);
}

void Transaction::Log(const String& tableName, const PageSlotID& data) {
  _wTables.insert(tableName);
  _pRManager->Log(tableName, data);
}

void Transaction::Abort() {
  // Alert: 只实现了对 Insert 操作的回滚
  _pRManager->Undo();
}

void Transaction::Commit() {
  for (auto& iter : _wTables) {
    _pRManager->ClearLog(iter);
  }
  _wTables.clear();
}
}  // namespace thdb
