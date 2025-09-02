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
    } else {
        std::cout << "Table created successfully." << std::endl;
    }
}

bool registerUser(sqlite3* db, const std::string& username, const std::string& password,
                  const std::string& fullname, const std::string& email, int age) {
    sqlite3_stmt* stmt;

    // Insert into users table
    std::string sqlUser = "INSERT INTO users (username, password) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, sqlUser.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare user insert: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting user: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    int userId = (int)sqlite3_last_insert_rowid(db);

    // Insert into members table
    std::string sqlMember =
        "INSERT INTO members (user_id, fullname, email, age) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sqlMember.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare member insert: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, fullname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, age);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting member: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "User registered successfully!" << std::endl;
    return true;
}
