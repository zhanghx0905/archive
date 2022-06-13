#include "manager/transaction_manager.h"

#include "manager/recovery_manager.h"

namespace thdb {

Transaction *TransactionManager::Begin() {
  auto txn = new Transaction(_txCounter, _activeTxSet, _pRManager);
  _activeTxSet.insert(_txCounter);
  _txCounter++;
  return txn;
}

void TransactionManager::Commit(Transaction *txn) {
  _activeTxSet.erase(txn->GetID());
  txn->Commit();
}

void TransactionManager::Abort(Transaction *txn) {
  _activeTxSet.erase(txn->GetID());
  txn->Abort();
}

}  // namespace thdb
