# Lab1 Report

2018011365 张鹤潇

### 任务概述

在本次实验中，我通读了框架代码，深入了解了框架的架构设计；完成了实验要求补全的函数并通过了所有测例。

我的快速查找非满记录页面算法实现如下：将所有非满页的页号放在哈希表 `_pageSet` 中。每次调用 `UpdateNotFull` 函数时传入被修改页的编号，如果该页已满，就把它从表中删除；如果该页变为空，就把它插入表中；如果表中没有可用页，就创建新的 `LinkedPage`.

因精力有限，我并没有完成可变长记录页的实现。

### 关键思路和难点

在实验中遇到的主要 bug 是 `Page` 的写后写问题。因 `Page` 对象在被析构时才会被写回到其对应的页内存中，如果同时拥有两个指向同一页的 `Page` 指针，其中一个指针所做的修改很可能被另一个覆盖掉。

相关的两个函数实现如下：

```c++
PageSlotID Table::InsertRecord(Record *pRecord) {
  // DONE: 插入一条记录
  SlotID slotID = 0;
  auto pPage = make_unique<RecordPage>(_nNotFull);
  auto dst = make_unique<uint8_t[]>(pTable->GetTotalSize());
  pRecord->Store(dst.get());
  slotID = pPage->InsertRecord(dst.get());
  // 在 UpdateNotFull 中会再次获取 _nNotFull 页的指针
  // 为了避免冲突，需要在调用前 UpdateNotFull 释放指针
  pPage.reset();	
  UpdateNotFull(_nNotFull);
  return PageSlotID(_nNotFull, slotID);
}

void Table::DeleteRecord(PageID nPageID, SlotID nSlotID) {
  // DONE: 删除一条记录
  make_unique<RecordPage>(nPageID)->DeleteRecord(nSlotID);
  // 临时指针对象已被析构，可以安全地调用 UpdateNotFull
  UpdateNotFull(nPageID);
}
```

另外需要注意，向 `Table::SearchRecord` 传入空指针表示查找表中所有记录。

