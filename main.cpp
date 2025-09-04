#include "db.h"
#include <iostream>

int main() {
    sqlite3* db = openDatabase("test.db");
    if (!db) return 1;

    if (registerUser(db)) {
        std::cout << "Registration complete.\n";
    } else {
        std::cout << "Registration failed.\n";
    }

    closeDatabase(db);
    return 0;
}
