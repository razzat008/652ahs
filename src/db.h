#ifndef DB_H
#define DB_H

#include "../libs/config.h"

class Database {
public:
  Database(const std::string &dbName);
  ~Database();
  bool init();
  bool insertData(const std::string &text, const std::string &hash,
                  const std::string &timestamp,
                  const std::string &file_name = "none",
                  const double file_size = 0);
  bool getEntry(const std::string &field, std::string &result);
  bool getEntry(const std::string &field, double &result);

private:
  sqlite3 *db;
  std::string dbName;
};

#endif // DB_H
