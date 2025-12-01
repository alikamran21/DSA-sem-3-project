#include <iostream>
#include <fstream>
#include <vector>
#include <termios.h> // For SSH/Terminal raw mode
#include <unistd.h>
#include <cmath>
#include <chrono>

// --- EXISTING INCLUDES ---
#include "avl_profile.h"
#include "queue_monitor.h"
#include "stack_monitor.h"

using namespace std;
using namespace std::chrono;

// Configuration to handle Terminal I/O
struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode (no Enter needed)
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    cout << "--- BIOMETRIC TRAINING PHASE (SSH MODE) ---" << endl;
    cout << "Note: Mouse dynamics are disabled in SSH mode." << endl;
    cout << "Initializing systems..." << endl;

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
            
            profile.insertOrUpdate("Keystroke_Dynamics", latency);

            // --- INTEGRATION: Log to Queue and Stack ---
            UserAction act("TrainerUser", "KeyPress", "SSH_Keyboard", latency);
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
    profile.exportToCSV("bio_fingerprints.csv");
    cout << "Saved to 'bio_fingerprints.csv'." << endl;

    return 0;
}