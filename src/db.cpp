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
                          const std::string &timestamp, const double file_size,
                          const std::string &file_name) {

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

// bool Database::getEntry(const std::string &query) {
//   const char *sql =
//       "SELECT ? FROM Hashes;";
//   sqlite3_stmt *stmt;
//   if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//     std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
//               << std::endl;
//     return false;
//   }
//
//   sqlite3_bind_text(stmt,1,sql,-1,SQLITE_STATIC);
//
//   if (sqlite3_step(stmt) == SQLITE_ROW) {
//     filename = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
//     filesize = sqlite3_column_double(stmt, 1);
//     timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
//     hash = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
//   } else {
//     std::cerr << "Failed to retrieve entry: " << sqlite3_errmsg(db)
//               << std::endl;
//     sqlite3_finalize(stmt);
//     return false;
//   }
//
//   sqlite3_finalize(stmt);
//   return true;
// }
bool Database::getEntry(const std::string &field, std::string &result) {
  // Construct SQL query to get the latest entry based on the highest ID
  std::string sql =
      "SELECT " + field + " FROM Hashes ORDER BY ID DESC LIMIT 1;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    result = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));

  } else {
    std::cerr << "Failed to retrieve entry: " << sqlite3_errmsg(db)
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
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }
  int rc = sqlite3_step(stmt);

  if (rc == SQLITE_ROW) {
    result = sqlite3_column_double(stmt, 0);
  } else if (rc == SQLITE_DONE) {

    std::cerr << "No entries in the table" << std::endl;
    result = 0.0; // or set to some default value

  } else {

    std::cerr << "Failed to retrieve entry: " << sqlite3_errmsg(db)
              << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }
  sqlite3_finalize(stmt);
  return true;
}
