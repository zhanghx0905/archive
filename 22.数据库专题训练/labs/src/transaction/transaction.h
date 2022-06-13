#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include <unordered_map>
#include <unordered_set>

#include "defines.h"
#include "table/table.h"

namespace thdb {

class RecoveryManager;

class Transaction {
 public:
  explicit Transaction(TxnID txnID,
                       const std::unordered_set<TxnID>& activeTxSet,
                       RecoveryManager* pRManager);
  ~Transaction() = default;
  TxnID GetID() const { return _txnID; }
  void Log(const String& tableName, const PageSlotID& pageSlot);
  void Abort();
  void Commit();
  bool CanSee(TxnID txnID) const;

 private:
  TxnID _txnID;
  std::unordered_set<TxnID> _activeTxSet;
  std::unordered_set<String> _wTables;
  RecoveryManager* _pRManager;
};

}  // namespace thdb

#endif  // TRANSACTION_H_
