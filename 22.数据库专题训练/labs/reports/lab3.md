# Lab3 Report

2018011365 张鹤潇

### 任务概述

在本次实验中，我实现了多表的 Sort Merge JOIN, 通过了所有测例。

### 关键思路

Sort Merge JOIN 算法实现是本实验的主要难点, 其中 `Merge` 部分代码如下:

```c++
  Size begin = 0; // 标记内循环的起始点
  for (Record *recordA : recordsA) {
    bool flag = true;
    for (Size idx = begin; idx < recordsB.size(); ++idx) {
      if (recordEq(recordA, recordsB[idx])) {
        Record *tmp = recordA->Copy();
        tmp->Add(recordsB[idx]);
        records.emplace_back(tmp);
        if (flag) {
          // 记录第一次相等的位置, 作为下一轮外循环中内循环的起始点.
          begin = idx;
          flag = false;
        }
      } else if (recordLess(recordA, recordB)) {
        // 进行下一轮外循环
        break;
      }
    }
  }
```

多表 Join 也是一个难点，我以通过附加测例为目标，采取了从右到左的简单执行计划。

用 `set<string> mergedTables ` 维护已 JOIN 过的表名，JOIN 时如果当前表已被合并过，就以中间结果作为当前表的 `Results`：

```c++
  if (mergedTables.count(joinCond->sTableA)) {
      recordsA = std::move(records);
    } else {
      mergedTables.insert(joinCond->sTableA);
      for (auto &pageSlot : iResultMap[joinCond->sTableA]) {
        recordsA.emplace_back(pTableA->GetRecord(pageSlot.first, pageSlot.second));
      }
    }
```
