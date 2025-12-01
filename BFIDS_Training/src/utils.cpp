#include "utils.h"
using namespace std;

// Function: currentTime()
// Purpose: Get the current system time in a readable string format (like "Sat Oct 18 23:42:00 2025")
string currentTime()
{
    time_t now = time(0);      // Get the current time in seconds
    char *dt = ctime(&now);    // Convert it to a human-readable string (adds a newline at the end)
    
    string s(dt);              // Convert C-style string to a C++ string
    s.pop_back();              // Remove the trailing newline '\n' added by ctime()
    
    return s;                  // Return the formatted time as a string
}
