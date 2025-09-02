#include "db.h"
#include <iostream>

int main() {
    sqlite3* db = openDatabase("test.db");
    if (!db) return 1;

    std::cout << "=== User Registration ===\n";
    std::string username, password, fullname, email;
    int age;

    std::cout << "Enter Username: ";
    std::cin >> username;

    std::cout << "Enter Password: ";
    std::cin >> password;

    std::cin.ignore();
    std::cout << "Enter Full Name: ";
    std::getline(std::cin, fullname);

    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    std::cout << "Enter Age: ";
    std::cin >> age;

    if (registerUser(db, username, password, fullname, email, age)) {
        std::cout << "Registration complete.\n";
    } else {
        std::cout << "Registration failed.\n";
    }

    closeDatabase(db);
    return 0;
}
