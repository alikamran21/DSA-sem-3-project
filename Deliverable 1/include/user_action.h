#ifndef USER_ACTION_H
#define USER_ACTION_H

#include <string>
#include <ctime>
using namespace std;

// Represents one user action log entry
struct UserAction {
    string timestamp;
    string userID;
    string action;
    string status;

    // Constructor for quick initialization
    UserAction(string t = "", string u = "", string a = "", string s = "");

    // Static helper to get current system time
    static string getCurrentTime();
};

#endif // USER_ACTION_H
