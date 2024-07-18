#include "db.h"
#include <iostream>

Database::Database(const std::string &dbName) : db(nullptr), dbName(dbName) {}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::init() {
    if (sqlite3_open(dbName.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    const char *sql = "CREATE TABLE IF NOT EXISTS Hashes ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "Text TEXT NOT NULL, "
                      "Hash TEXT NOT NULL, "
                      "Timestamp TEXT NOT NULL);";
    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::insertData(const std::string &text, const std::string &hash, const std::string &timestamp) {
    const char *sql = "INSERT INTO Hashes (Text, Hash, Timestamp) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, text.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, timestamp.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
