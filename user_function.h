#ifndef USERFUNCS_H
#define USERFUNCS_H

#include <sqlite3.h>
#include <string>

void showMenu();
bool registerUser(sqlite3* db);
bool loginUser(sqlite3* db);
void logoutUser();
int printCallback(void* NotUsed, int argc, char** argv, char** azColName);

// User-level features
void addSkill(sqlite3* db);
void viewSkills(sqlite3* db);
void showUserMenu(sqlite3* db);

// Admin functions
void adminFunction(sqlite3* db);
void showAdminMenu(sqlite3* db);
#endif