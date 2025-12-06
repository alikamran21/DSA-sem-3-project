#include <iostream>
#include <fstream>
#include <vector>
#include <termios.h> // For SSH/Terminal raw mode
#include <unistd.h>
#include <cmath>
#include <chrono>
#include <string>
#include <cstdlib> // Added for system()

// --- INCLUDES ---
#include "avl_profile.h"
#include "queue_monitor.h"
#include "stack_monitor.h"

using namespace std;
using namespace std::chrono;

/*
    bio_trainer.cpp
    ---------------
    The Training Module for the BFIDS system (SSH Mode).
    
    Functionality:
    1. Captures user keystrokes in real-time.
    2. Measures flight time (latency) between keys.
    3. Builds a statistical profile (Average Duration).
    4. Logs training events to Stack and Queue monitors for analysis.
    5. Exports the trained profile to a CSV file.
*/

// Configuration to handle Terminal I/O
struct termios orig_termios;

/*
    disableRawMode
    --------------
    Resets terminal settings to default.
*/
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/*
    enableRawMode
    -------------
    Enables raw input mode for accurate keystroke timing.
    Disables canonical mode (wait for Enter) and echo.
*/
void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode (no Enter needed)
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/*
    main
    ----
    Main training loop.
    Captures input, calculates metrics, updates AVL Tree, and saves data.
*/
int main() {
    cout << "--- BIOMETRIC TRAINING PHASE (SSH MODE) ---" << endl;
    
    // Ask for Username
    string username;
    cout << "Enter username for training (e.g., admin): ";
    cin >> username;
    
    // Clear input buffer to prevent 'Enter' from being registered as a keystroke
    cin.ignore(); 

    cout << "Note: Mouse dynamics are disabled in SSH mode." << endl;
    cout << "Initializing systems for user: [" << username << "]..." << endl;

    AVLProfile profile;

    // --- INTEGRATION: Initialize Monitors ---
    QueueMonitor eventQueue;
    StackMonitor eventStack;
    cout << "[System] Event Queue and Stack Monitors initialized." << endl;

    // Enable raw mode to catch individual keystrokes
    enableRawMode();
    
    cout << "\n>>> TRAINER ACTIVE. TYPE NORMALLY." << endl;
    cout << ">>> (Press 'ESC' (or Ctrl+[) to finish training)" << endl;

    char c;
    auto lastKeyTime = high_resolution_clock::now();
    bool firstKey = true;

    // Data Capture Loop
    while (read(STDIN_FILENO, &c, 1) == 1) {
        auto now = high_resolution_clock::now();
        double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
        lastKeyTime = now;

        // Exit on ESC (ASCII 27)
        if (c == 27) {
            break;
        }

        // We skip the very first key because we don't have a previous time reference
        if (!firstKey && latency < 2000) {
            cout << "\r[KEY] Latency: " << latency << "ms   " << flush; // \r overwrites line
            
            // Update the statistical model in the AVL Tree
            profile.insertOrUpdate("Keystroke_Dynamics", latency);

            // --- INTEGRATION: Log to Queue and Stack ---
            // Use dynamic username
            UserAction act(username, "KeyPress", "SSH_Keyboard", latency);
            eventQueue.enqueueAction(act);
            eventStack.pushAction(act);
        }
        firstKey = false;
    }

    // Restore terminal before printing summary
    disableRawMode();

    cout << "\n\nTraining Complete." << endl;
    
    // --- INTEGRATION: Show stats ---
    cout << "Total Events Queued: " << eventQueue.getSize() << endl;
    cout << "Total Events Stacked: " << eventStack.getSize() << endl;

    cout << "Saving Profile..." << endl;
    
    // --- CHANGE: Create directory and update path ---
    system("mkdir -p fingerprints");
    string filename = "fingerprints/bio_fingerprints_" + username + ".csv";
    
    // Serialize the AVL Tree to CSV
    profile.exportToCSV(filename);
    cout << "Saved to '" << filename << "'." << endl;

    return 0;
}