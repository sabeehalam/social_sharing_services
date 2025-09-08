#include "db.h"
#include "user_function.h"
#include <iostream>

// Extern global variable from db.cpp
extern int currentUserId;
extern int currentUserRole;

int main() {
    sqlite3* db = openDatabase("test.db");
    if (!db) return 1;

    int choice;
    bool running = true;
    while (running) {
        std::cout << "\n=== Main Menu ===\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. Logout\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose: ";
        std::cin >> choice;

        switch (choice) {
            case 1: registerUser(db); break;
            case 2: if (loginUser(db)) {
                        if (currentUserRole == "admin") showAdminMenu(db);
                        else showUserMenu(db);
                    }
                    break;
            case 3: logoutUser(); break;
            case 4: running = false; break;
            default: std::cout << "Invalid choice.\n"; break;
        }
    }

    closeDatabase(db);
    return 0;
}
