#include "user_function.h"
#include <iostream>

// Global variable to track current logged-in user
int currentUserId = -1;

void showMenu() {
    std::cout << "\n=== Social Community Project ===\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Logout\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

bool registerUser(sqlite3* db) {
    std::cout << "=== User Registration ===\n";
    std::string username, password, fullname, email;
    int age;

    std::cout << "Enter Username: ";
    std::cin >> username;

    std::cout << "Enter Password: ";
    std::cin >> password;

    std::cin.ignore();
    std::cout << "Enter Full Name: ";
    std::getline(std::cin, fullname);

    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    std::cout << "Enter Age: ";
    std::cin >> age;

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
    std::cout << "✅ User registered successfully!\n";
    return true;
}

bool loginUser(sqlite3* db) {
    std::cout << "=== User Login ===\n";
    std::string username, password;

    std::cout << "Enter Username: ";
    std::cin >> username;

    // ✅ Step 1: Check if username exists
    sqlite3_stmt* checkStmt;
    std::string checkSql = "SELECT id FROM users WHERE username = ?;";
    if (sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare username check: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_text(checkStmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "⚠️ Username not found. Please register first.\n";
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);

    // ✅ Step 2: Ask for password
    std::cout << "Enter Password: ";
    std::cin >> password;

    // ✅ Step 3: Verify username + password
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id FROM users WHERE username = ? AND password = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare login query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        currentUserId = sqlite3_column_int(stmt, 0);
        std::cout << "✅ Login successful! Welcome, " << username << ".\n";
        sqlite3_finalize(stmt);
        return true;
    } else {
        std::cout << "❌ Incorrect password.\n";
        sqlite3_finalize(stmt);
        return false;
    }
}

void logoutUser() {
    if (currentUserId != -1) {
        std::cout << "👋 User logged out successfully.\n";
        currentUserId = -1;
    } else {
        std::cout << "⚠️ No user is currently logged in.\n";
    }
}

int printCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
    }
    std::cout << std::endl;
    return 0;
}