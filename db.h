// db.h
#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <string>

// Opens a connection
sqlite3* openDatabase(const std::string& filename);

// Creates a table
void createTable(sqlite3* db);

// Inserts a record
void insertUser(sqlite3* db, int id, const std::string& name);

// Closes the connection
void closeDatabase(sqlite3* db);

#endif