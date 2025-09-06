#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <string>

sqlite3* openDatabase(const std::string& filename);
void closeDatabase(sqlite3* db);

// Generic table creation
void createTable(sqlite3* db, const std::string& createQuery);

// User registration
bool registerUser(sqlite3* db);

// User login
bool loginUser(sqlite3* db);

// User logout
void logoutUser();

bool executeNonQuery(sqlite3* db, const std::string& query);

bool executeSelect(sqlite3* db, const std::string& query, int (*callback)(void*, int, char**, char**), void* data);

#endif
