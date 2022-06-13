# Lab4 Report

2018011365 张鹤潇

### 任务概述

在本次实验中，我实现了可通过 Insert、Delete、Update 测试的 MVCC。

### 关键思路和难点

实验的难点集中在对 `Instance` 类方法的改造。

对参考实现思路略作扩展，在 CreateTable 函数创建表时增加两列，分别储存记录的创建和删除时间；

调用 Insert 函数插入记录时，在这两列储存相应信息，其中创建时间为执行插入操作的事务 ID，删除时间初始化为 `INT_MAX`.

调用 Delete 函数删除记录时，将记录的删除时间修改为进行操作的事务 ID.

```c++
// Instance::Delete
pTable->UpdateRecord(iPair.first, iPair.second,
                       {Transform(txnEndPos, FieldType::INT_TYPE,
                                  std::to_string(txn->GetID()))});
```

调用 Update 函数更新记录时，将旧记录的删除时间修改为执行更新操作的事务 ID，创建一条新纪录，其创建时间为执行更新操作的事务 ID.

在查找记录时，根据当前事务的 ID、 记录的创建事务 ID、删除事务 ID，以及当前事务开始时的活跃事务来判断该记录是否可见，对结果进行过滤。

```c++
// _txnID: 当前事务 ID
// _activeTxSet: 当前事务开始时的活跃事务集合
bool Transaction::CanSee(TxnID txnID) const {
  return _txnID == txnID || (txnID < _txnID && _activeTxSet.count(txnID) == 0);
}

// Instance::Search
if (txn->CanSee(txnBeg) && !txn->CanSee(txnEnd)) {
  // 该记录对当前事务可见
} else {
  // 该记录对当前事务不可见
}
```

为了在事务 Abort 后删除插入的条目，我在 `Transaction` 类中储存了修改过的 Record 信息，在 Abort 时删除这些 Record.

### Mysql 中的 MVCC

> [一文理解Mysql MVCC - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/66791480)

基于 Undo Log 和 Read View，不详细写了。