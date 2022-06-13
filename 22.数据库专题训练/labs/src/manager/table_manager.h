#ifndef THDB_TABLE_MANAGER_H_
#define THDB_TABLE_MANAGER_H_

#include "defines.h"
#include "exception/table_exception.h"
#include "macros.h"
#include "table/schema.h"
#include "table/table.h"

namespace thdb {

using TableLogID = std::pair<PageID, PageID>;

class TableManager {
 public:
  TableManager();
  ~TableManager();

  Table *GetTable(const String &sTableName);
  Table *AddTable(const String &sTableName, const Schema &iSchema);
  void DropTable(const String &sTableName);

  std::vector<String> GetTableNames() const;
  std::vector<String> GetColumnNames(const String &sTableName);
  PageID GetLogPageID(const String &sTableName) {
    if (_iTableIDMap.count(sTableName) > 0)
      return _iTableIDMap[sTableName].second;
    throw TableNotExistException(sTableName);
  }

 private:
  std::map<String, Table *> _iTableMap;
  std::map<String, TableLogID> _iTableIDMap;

  void Store();
  void Load();
};

}  // namespace thdb

#endif