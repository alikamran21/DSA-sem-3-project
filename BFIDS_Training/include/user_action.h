#ifndef USER_ACTION_H
#define USER_ACTION_H
#include <string>
#include <ctime>
using namespace std;

/*
 * UserAction
 * ----------
 * Represents a single user action event.
 * Used in:
 * - Logging
 * - Analysis (Sorting, Stacks, Queues)
 * - Anomaly Detection
 *
 * Fields:
 * userID, action, processName, duration, timestamp, nextAction, status
 */

// Defines a structure for tracking a single user action.
struct UserAction {
    string userID;        // ID of the user.
    string action;        // Type of action (e.g., "Login").
    string processName;   // Application/process involved.
    double duration;      // Action duration (double).
    time_t timestamp;     // Action time (time_t).
    string nextAction;    // Next expected action.
    string status;        // Status (e.g., "Normal", "Error").

    // Constructor with default values.
    // Order: ID, Action, Process, Duration, Timestamp, Next Action, Status.
    UserAction(string id = "", string act = "", string proc = "", double dur = 0.0,
               time_t ts = std::time(nullptr), string next = "", string stat = "Normal")
        : userID(id), action(act), processName(proc), duration(dur),
          timestamp(ts), nextAction(next), status(stat) {} 
};

#endif