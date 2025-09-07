#include "db.h"
#include <iostream>

sqlite3* openDatabase(const std::string& filename) {
    sqlite3* db;
    if (sqlite3_open(filename.c_str(), &db) == SQLITE_OK) {
        std::cout << "Opened database: " << filename << std::endl;
        return db;
    } else {
        std::cerr << "Failed to open database: " << filename << std::endl;
        return nullptr;
    }
}

void closeDatabase(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
        std::cout << "Database closed." << std::endl;
    }
}

void createTable(sqlite3* db, const std::string& createQuery) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, createQuery.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Run queries that don't return rows (CREATE, INSERT, UPDATE, DELETE)
bool executeNonQuery(sqlite3* db, const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Run SELECT queries with a callback
bool executeSelect(sqlite3* db, const std::string& query, int (*callback)(void*, int, char**, char**), void* data) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), callback, data, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}