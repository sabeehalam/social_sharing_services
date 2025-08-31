#include <iostream>
#include "sqlite3.h"

using namespace std;

int main() {
    sqlite3 *db;
    if (sqlite3_open("mydb.db", &db) == 0) {
        cout<<"Opened database successfully\n"<<endl;
        sqlite3_close(db);
    }
    cout << "What would you like to do right now ?" << endl;
    return 0;
}   