#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include "avl_profile.h"

using namespace std;

/*
    live_monitor.cpp
    ----------------
    A simulation/testing version of the monitoring system.
    Allows manual entry of actions and durations to test detection logic 
    without needing real-time biometric hardware/SSH.
    
    Features:
    - Manual data entry simulation.
    - Anomaly detection based on imported CSV profiles.
    - Simulated shutdown trigger.
*/

// CONFIGURATION
// --- CHANGE: Path updated ---
const string FINGERPRINT_FILE = "fingerprints/fingerprints.csv";
const int ANOMALY_THRESHOLD = 10; // High score = Shutdown

/*
    triggerShutdown
    ---------------
    Executes the system shutdown command when a threat is confirmed.
    Requires sudo privileges.
*/
void triggerShutdown() {
    cout << "\n[!!!] RED ALERT: MALICIOUS ACTIVITY DETECTED [!!!]" << endl;
    cout << "[!!!] The system is compromised. Initiating EMERGENCY SHUTDOWN." << endl;
    
    // The actual Linux command to kill the machine
    // We use 'sudo' so it works (you must run the monitor as sudo)
    system("sudo shutdown -h now"); 
}

/*
    main
    ----
    Simulation loop.
    1. Loads the fingerprint.
    2. Accepts manual input for Action and Duration.
    3. Calculates threat score.
    4. Triggers shutdown if threshold is met.
*/
int main() {
    cout << "============================================" << endl;
    cout << "   BFIDS LIVE MONITORING SYSTEM (ACTIVE)    " << endl;
    cout << "============================================" << endl;

    // 1. Load the "Brain" (The patterns we learned from the static file)
    AVLProfile validProfile;
    if (!validProfile.importFromCSV(FINGERPRINT_FILE)) {
        cerr << "[ERROR] No training data found! Run ./trainer first." << endl;
        return 1;
    }
    cout << "[System] Behavioral Patterns Loaded. Watching for anomalies...\n" << endl;

    // 2. The Infinite Loop (This is the "Live" part)
    string action;
    double duration;
    int currentThreatLevel = 0;

    while (true) {
        // A. Wait for real-time input
        cout << "\n[LIVE INPUT] Enter Action (or 'exit'): ";
        cin >> action;
        if (action == "exit") break;

        cout << "[LIVE INPUT] How long did it take (ms)? ";
        cin >> duration;

        // B. Compare against the Profile
        UserActionProfile stats;
        bool known = validProfile.search(action, stats);

        if (!known) {
            cout << "   -> [ALERT] UNKNOWN BEHAVIOR! User has never done '" << action << "' before." << endl;
            currentThreatLevel += 5;
        } else {
            // Check if they are doing it way too slow/fast (3x deviation)
            if (duration > stats.avgDuration * 3.0) {
                cout << "   -> [WARNING] TIMING ANOMALY! Action took " << duration << "ms (Expected: ~" << stats.avgDuration << "ms)" << endl;
                currentThreatLevel += 3;
            } else {
                cout << "   -> [OK] Behavior matches normal profile." << endl;
                if (currentThreatLevel > 0) currentThreatLevel--; // Cool down
            }
        }

        cout << "   -> Current Threat Score: " << currentThreatLevel << "/10" << endl;

        // C. The Consequence
        if (currentThreatLevel >= ANOMALY_THRESHOLD) {
            triggerShutdown();
            break; 
        }
    }

    return 0;
}