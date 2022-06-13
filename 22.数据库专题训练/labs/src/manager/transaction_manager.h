#ifndef TRANSACTION_MANAGER_H_
#define TRANSACTION_MANAGER_H_

#include <unordered_set>

#include "defines.h"
#include "transaction/transaction.h"

namespace thdb {

class RecoveryManager;

class TransactionManager {
 public:
  TransactionManager(RecoveryManager *pRManager) : _pRManager(pRManager) {}
  ~TransactionManager() = default;

  Transaction *Begin();
  void Commit(Transaction *txn);
  void Abort(Transaction *txn);

 private:
  std::unordered_set<TxnID> _activeTxSet = {};
  TxnID _txCounter = 0;
  RecoveryManager *_pRManager;
};

}  // namespace thdb

#endif  // TRANSACTION_MANAGER_H_
