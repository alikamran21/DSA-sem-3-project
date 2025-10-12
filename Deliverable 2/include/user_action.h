#ifndef USER_ACTION_H
#define USER_ACTION_H
#include <string>
#include <ctime>
using namespace std;

// Structure to represent a single user action entry
struct UserAction {
    string userID;        // Unique user ID
    string action;        // Type of action performed
    string processName;   // Name of process or app used
    double duration;      // Duration in milliseconds
    time_t timestamp;     // Time when action occurred
    string nextAction;    // Next expected action
    string status;        // Normal or Deviated

    // Constructor for initialization
    UserAction(string id = "", string act = "", string proc = "", double dur = 0.0,
               time_t time = std::time(nullptr), string next = "", string stat = "Normal")
        : userID(id), action(act), processName(proc), duration(dur),
          timestamp(time), nextAction(next), status(stat) {}
};

#endif // USER_ACTION_H
