#include "user_function.h"
#include <iostream>

// Global variable to track current logged-in user
int currentUserId = -1;        // logged-in user id
std::string currentUserRole;   // logged-in user role

void showMenu() {
    std::cout << "\n=== Social Community Project ===\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Logout\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

// ================= REGISTER =================
bool registerUser(sqlite3* db) {
    std::cout << "=== User Registration ===\n";
    std::string username, password, fullname, email, role;
    int age;

    std::cout << "Enter Username: ";
    std::cin >> username;

    // Check if username exists
    sqlite3_stmt* checkStmt;
    std::string checkSql = "SELECT id FROM users WHERE username = ?;";
    sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
        std::cout << "⚠️ Username already exists.\n";
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);

    std::cout << "Enter Password: ";
    std::cin >> password;

    std::cin.ignore();
    std::cout << "Enter Full Name: ";
    std::getline(std::cin, fullname);

    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    std::cout << "Enter Age: ";
    std::cin >> age;

    std::cout << "Enter Role (admin/user): ";
    std::cin >> role;
    if (role != "admin" && role != "user") role = "user";

    sqlite3_stmt* stmt;
    std::string sqlUser = "INSERT INTO users (username, password, role) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sqlUser.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting user: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    int userId = (int)sqlite3_last_insert_rowid(db);

    std::string sqlMember =
        "INSERT INTO members (user_id, fullname, email, age) VALUES (?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sqlMember.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, fullname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, age);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    std::cout << "✅ User registered successfully!\n";
    return true;
}

// ================= LOGIN =================
bool loginUser(sqlite3* db) {
    std::cout << "=== User Login ===\n";
    std::string username, password;

    std::cout << "Enter Username: ";
    std::cin >> username;

    // Check username exists
    sqlite3_stmt* stmt;
    std::string checkSql = "SELECT id FROM users WHERE username = ?;";
    sqlite3_prepare_v2(db, checkSql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        std::cout << "⚠️ Username not found.\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    std::cout << "Enter Password: ";
    std::cin >> password;

    // Validate username + password
    std::string sql = "SELECT id, role FROM users WHERE username = ? AND password = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        currentUserId = sqlite3_column_int(stmt, 0);
        currentUserRole = (const char*)sqlite3_column_text(stmt, 1);
        std::cout << "✅ Login successful! Role: " << currentUserRole << "\n";
        sqlite3_finalize(stmt);
        return true;
    } else {
        std::cout << "❌ Incorrect password.\n";
        sqlite3_finalize(stmt);
        return false;
    }
}

// ================= LOGOUT =================
void logoutUser() {
    if (currentUserId != -1) {
        std::cout << "👋 User logged out.\n";
        currentUserId = -1;
        currentUserRole.clear();
    } else {
        std::cout << "⚠️ No user logged in.\n";
    }
}

int printCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
    }
    std::cout << std::endl;
    return 0;
}

// =========== User Menu ===========
void showUserMenu(sqlite3* db) {
    int choice;
    do {
        std::cout << "\n=== User Menu ===\n";
        std::cout << "1. Add Skill\n";
        std::cout << "2. View Skills\n";
        std::cout << "3. Logout\n";
        std::cin >> choice;

        switch (choice) {
            case 1: addSkill(db); break;
            case 2: viewSkills(db); break;
            case 3: logoutUser(); break;
        }
    } while (choice != 3 && currentUserId != -1);
}

// =========== ADD SKILL ===========
void addSkill(sqlite3* db) {
    if (currentUserId == -1) {
        std::cout << "⚠️ Please login first.\n";
        return;
    }

    std::string skill, desc;
    std::cin.ignore();
    std::cout << "Enter Skill Name: ";
    std::getline(std::cin, skill);
    std::cout << "Enter Description: ";
    std::getline(std::cin, desc);

    std::string sql = "INSERT INTO skills (user_id, skill_name, description) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, currentUserId);
    sqlite3_bind_text(stmt, 2, skill.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, desc.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        std::cout << "✅ Skill added successfully!\n";
    else
        std::cout << "❌ Failed to add skill.\n";

    sqlite3_finalize(stmt);
}

// =========== VIEW SKILLS ===========
void viewSkills(sqlite3* db) {
    std::string sql = "SELECT skills.id, users.username, skills.skill_name, skills.description "
                      "FROM skills JOIN users ON skills.user_id = users.id;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::cout << "\n=== Community Skills ===\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string user = (const char*)sqlite3_column_text(stmt, 1);
        std::string skill = (const char*)sqlite3_column_text(stmt, 2);
        std::string desc = (const char*)sqlite3_column_text(stmt, 3);

        std::cout << id << ". [" << user << "] " << skill << " - " << desc << "\n";
    }
    sqlite3_finalize(stmt);
}

// ================= ADMIN ONLY EXAMPLE =================
void adminFunction(sqlite3* db) {
    std::cout << "=== Admin Function ===\n";
    std::cout << "This could list all users, delete users, etc.\n";
}

// =========== Admin Menu ===========
void showAdminMenu(sqlite3* db) {
    int choice;
    do {
        std::cout << "\n=== Admin Menu ===\n";
        std::cout << "1. Register User\n";
        std::cout << "2. Admin Function\n";
        std::cout << "3. Logout\n";
        std::cin >> choice;

        switch (choice) {
            case 1: registerUser(db); break;
            case 2: adminFunction(db); break;
            case 3: logoutUser(); break;
        }
    } while (choice != 4 && currentUserId != -1);
}
