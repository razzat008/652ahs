#include "db.h"
#include <iostream>
#include <string>

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
                    "Timestamp TEXT NOT NULL, "
                    "Filename TEXT , "
                    "Filesize REAL); ";
  char *errMsg = nullptr;
  if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
    std::cerr << "SQL error: " << errMsg << std::endl;
    sqlite3_free(errMsg);
    return false;
  }
  return true;
}

bool Database::insertData(const std::string &text, const std::string &hash,
                          const std::string &timestamp,
                          const std::string &file_name,
                          const double file_size) {

  const char *sql = "INSERT INTO Hashes (Text, Hash, Timestamp, Filename, "
                    "Filesize) VALUES (?, ?, ?, ?, ?);";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }
  sqlite3_bind_text(stmt, 1, text.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, hash.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, timestamp.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, file_name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_double(stmt, 5, file_size);
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db)
              << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }
  sqlite3_finalize(stmt);
  return true;
}

bool Database::getEntry(const std::string &field, double &result) {
  std::string sql =
      "SELECT " + field + " FROM Hashes ORDER BY ID DESC LIMIT 1;";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    throw std::runtime_error("Failed to prepare statement: " +
                             std::string(sqlite3_errmsg(db)));
  }

  int rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) { // when returning data
    result = sqlite3_column_double(stmt, 0);
  } else if (rc == SQLITE_DONE) {
    result = 0.0; // incase of an empty database
  } else {
    throw std::runtime_error("Failed to retrieve entry: " +
                             std::string(sqlite3_errmsg(db)));
  }
  sqlite3_finalize(stmt);
  return true;
}

bool Database::getEntry(const std::string &field, std::string &result) {
  // Construct SQL query to get the latest entry based on the highest ID
  std::string sql = "SELECT " + field + " FROM Hashes ORDER BY ID DESC LIMIT 1;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    const unsigned char *text = sqlite3_column_text(stmt, 0);
    if (text != nullptr) {
      result = reinterpret_cast<const char *>(text);
    } else {
      result = "";
    }
  } else {
    std::cerr << "Failed to retrieve entry: " << sqlite3_errmsg(db)
              << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  if (sqlite3_finalize(stmt) != SQLITE_OK) {
    std::cerr << "Failed to finalize statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }

  return true;
}
