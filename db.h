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

#endif
