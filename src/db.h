#ifndef DB_H
#define DB_H

#include "../libs/config.h"

class Database {
public:
    Database(const std::string &dbName);
    ~Database();
    bool init();
    bool insertData(const std::string &text, const std::string &hash, const std::string &timestamp);

private:
    sqlite3 *db;
    std::string dbName;
};

#endif // DB_H
