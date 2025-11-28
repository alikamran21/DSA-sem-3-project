#ifndef USER_ACTION_PROFILE_H
#define USER_ACTION_PROFILE_H

#include <string>
using namespace std;   // ← added exactly as you requested

/*
 * UserActionProfile
 * ------------------
 * Represents the statistical fingerprint for a single process.
 *
 * Used in:
 *  - Training Phase: store frequency + avgDuration
 *  - Live Detection: compare new events vs stored statistics
 */
struct UserActionProfile {
    string processName;   // Key used in the AVL tree
    int frequency;        // How many times process occurred
    double avgDuration;   // Rolling average duration

    // Constructor
    UserActionProfile(string name = "",
                      int freq = 0,
                      double avg = 0.0)
        : processName(name),
          frequency(freq),
          avgDuration(avg) {}
};

#endif // USER_ACTION_PROFILE_H
