#include "db.h"
#include "user_function.h"
#include <iostream>

// Extern global variable from db.cpp
extern int currentUserId;

int main() {
    sqlite3* db = openDatabase("test.db");
    if (!db) return 1;
    
    int choice;
    bool running = true;

    while (running) {
        showMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                registerUser(db);
                break;
            case 2:
                loginUser(db);
                break;
            case 3:
                logoutUser();
                break;
            case 4:
                running = false;
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
                break;
        }
    }


    closeDatabase(db);
    return 0;
}
