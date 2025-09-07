#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <string>

// Global variables for session
extern int currentUserId;
extern std::string currentUserRole;

// Database helpers
sqlite3* openDatabase(const std::string& filename);
void closeDatabase(sqlite3* db);
void createTable(sqlite3* db, const std::string& createQuery);

// Authentication
bool registerUser(sqlite3* db);
bool loginUser(sqlite3* db);
void logoutUser();

bool executeNonQuery(sqlite3* db, const std::string& query);
bool executeSelect(sqlite3* db, const std::string& query, int (*callback)(void*, int, char**, char**), void* data);


#endif // DB_H
