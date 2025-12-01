#include <iostream>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <cstdlib> 
#include <termios.h>

// --- EXISTING INCLUDES ---
#include "avl_profile.h"
#include "Array_handler.h"
#include "pointer_utils.h"
#include "utils.h"
#include "hash_profiles.h"
#include "graph_transition.h"

using namespace std;
using namespace std::chrono;

const int ANOMALY_LIMIT  = 20;

// Terminal handling
struct termios orig_termios;
void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }
void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void triggerLockdown() {
    disableRawMode(); // Restore terminal so we can see the error
    cout << "\n\033[1;41m[!!!] BIOMETRIC MISMATCH DETECTED [!!!]\033[0m" << endl;
    cout << "System assumes unauthorized user. Exiting session..." << endl;
    exit(1);
}

int main() {
    // --- INTEGRATION: Utils & Pointer Check ---
    cout << "System started at: " << currentTime() << endl;
    PointerUtils::showMemoryUsage();

    // 1. Load Biometric Profile
    AVLProfile validProfile;
    if (!validProfile.importFromCSV("bio_fingerprints.csv")) {
        cerr << "Error: No biometric profile found. Run bio_trainer first!" << endl;
        return 1;
    }

    // --- INTEGRATION: Hash Table & Array Handler ---
    hashTable userRegistry(10);
    userRegistry.addProfile("TargetUser", &validProfile);

    arrayHandler anomalyLog; 
    GraphTransition sysState;
    sysState.addTransition("Safe", "Anomaly");
    sysState.addTransition("Anomaly", "Lockdown");
    string currentState = "Safe";
    
    UserActionProfile keyStats;
    AVLProfile* activeProfile = userRegistry.getProfile("TargetUser");
    bool hasKeys = activeProfile->search("Keystroke_Dynamics", keyStats);

    if (!hasKeys) {
        cerr << "Error: No keystroke data found in fingerprint." << endl;
        return 1;
    }

    cout << "--- BIOMETRIC LIVE MONITORING (SSH MODE) ---" << endl;
    cout << "Avg Key Latency: " << keyStats.avgDuration << "ms" << endl;
    cout << "Start typing to verify identity... (ESC to quit)" << endl;

    enableRawMode();

    char c;
    auto lastKeyTime = high_resolution_clock::now();
    int anomalyScore = 0;
    bool firstKey = true;

    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 27) break; // ESC to quit

        auto now = high_resolution_clock::now();
        double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
        lastKeyTime = now;

        if (!firstKey && latency < 2000) { 
            // Check limits (Allowing 0.3x to 3.0x deviation)
            if (latency < keyStats.avgDuration * 0.3 || latency > keyStats.avgDuration * 3.0) {
                
                // Print Red Warning (Using \r to stay on line)
                cout << "\r\033[31m[!] Anomaly: " << (int)latency << "ms (Score: " << anomalyScore+2 << ")\033[0m   " << flush;
                anomalyScore += 2;
                
                // --- INTEGRATION: Log Anomaly ---
                anomalyLog.addTask("KeyAnomaly_" + to_string((int)latency) + "ms");
                
                // --- INTEGRATION: Pointer Check ---
                void* ptr = PointerUtils::allocate(sizeof(int), "AnomalyFlag");
                if(PointerUtils::isValidPointer(ptr)) PointerUtils::deallocate(ptr, "AnomalyFlag");

            } else {
                // Print Green OK
                cout << "\r\033[32m[OK] Match: " << (int)latency << "ms      \033[0m" << flush;
                if (anomalyScore > 0) anomalyScore--;
            }
        }
        firstKey = false;

        // C. CHECK SCORE
        if (anomalyScore > 0) {
            if (currentState == "Safe" && sysState.isValidTransition("Safe", "Anomaly")) {
                currentState = "Anomaly";
            }
        } else {
             currentState = "Safe";
        }

        if (anomalyScore >= ANOMALY_LIMIT) {
            if (sysState.isValidTransition(currentState, "Lockdown")) {
                disableRawMode(); // Restore terminal so logs print cleanly
                cout << "\nDumping Anomaly Logs:" << endl;
                anomalyLog.showTaskList();
                triggerLockdown();
            }
        }
    }

    disableRawMode();
    return 0;
}