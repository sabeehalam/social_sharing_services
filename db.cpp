#include <sqlite3.h>
#include <iostream>
#include <string>
using namespace std;

sqlite3* DB;

// Utility function to execute SQL
int execSQL(const string& sql) {
    char* errorMessage;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "SQL Error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return 1;
    }
    return 0;
}

// Initialize database and create Users table
void initDB() {
    int exit = sqlite3_open("community.db", &DB);
    if (exit) {
        cerr << "Error opening DB: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    string sql = "CREATE TABLE IF NOT EXISTS USERS("
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "USERNAME TEXT UNIQUE, "
                 "PASSWORD TEXT, "
                 "NEIGHBORHOOD TEXT);";

    execSQL(sql);
}
