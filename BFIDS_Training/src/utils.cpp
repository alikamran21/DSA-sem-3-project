#include "utils.h"
using namespace std;

/*
    utils.cpp
    ---------
    General utility functions used across the project.
*/

/*
    currentTime
    -----------
    Returns the current system time as a readable string.
    Format example: "Sat Oct 18 23:42:00 2025".
    Removes the trailing newline added by standard ctime().
*/
string currentTime()
{
    time_t now = time(0);      // Get the current time in seconds
    char *dt = ctime(&now);    // Convert it to a human-readable string (adds a newline at the end)
    
    string s(dt);              // Convert C-style string to a C++ string
    s.pop_back();              // Remove the trailing newline '\n' added by ctime()
    
    return s;                  // Return the formatted time as a string
}