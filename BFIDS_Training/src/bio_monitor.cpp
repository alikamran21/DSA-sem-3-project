#include <iostream>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <cstdlib> 

// --- EXISTING INCLUDES ---
#include "avl_profile.h"

// --- NEW INTEGRATIONS (Update paths as needed) ---
#include "Array_handler.h"
#include "pointer_utils.h"
#include "utils.h"
#include "hash_profiles.h"
#include "graph_transition.h"

using namespace std;
using namespace std::chrono;

// --- CONFIGURATION ---
const char* KEYBOARD_DEV = "/dev/input/event2"; 
const char* MOUSE_DEV    = "/dev/input/event3"; 
const int ANOMALY_LIMIT  = 20;

void triggerLockdown() {
    cout << "\n\033[1;41m[!!!] BIOMETRIC MISMATCH DETECTED [!!!]\033[0m" << endl;
    cout << "System assumes unauthorized user. Shutting down..." << endl;
    // system("sudo shutdown -h now"); // Commented out to prevent accidental shutdown during testing
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
    // Register the loaded profile into the Hash Table
    hashTable userRegistry(10);
    userRegistry.addProfile("TargetUser", &validProfile);
    cout << "[System] Profile loaded into Hash Table for 'TargetUser'." << endl;

    // Handler to store anomaly detection logs
    arrayHandler anomalyLog; 

    // Graph to track system state (Safe <-> Anomaly)
    GraphTransition sysState;
    sysState.addTransition("Safe", "Anomaly");
    sysState.addTransition("Anomaly", "Lockdown");
    string currentState = "Safe";
    
    // Get stats for comparison
    UserActionProfile keyStats, mouseStats;
    // Retrieve via Hash Table just to show usage
    AVLProfile* activeProfile = userRegistry.getProfile("TargetUser");
    bool hasKeys = activeProfile->search("Keystroke_Dynamics", keyStats);
    bool hasMouse = activeProfile->search("Mouse_Dynamics", mouseStats);

    if (!hasKeys) cout << "Warning: No keystroke data learned." << endl;
    if (!hasMouse) cout << "Warning: No mouse data learned." << endl;

    cout << "--- BIOMETRIC LIVE MONITORING ACTIVE ---" << endl;
    cout << "Avg Key Latency: " << keyStats.avgDuration << "ms" << endl;

    int kbd_fd = open(KEYBOARD_DEV, O_RDONLY | O_NONBLOCK);
    int mouse_fd = open(MOUSE_DEV, O_RDONLY | O_NONBLOCK);
    
    struct input_event ev;
    auto lastKeyTime = high_resolution_clock::now();
    int anomalyScore = 0;

    while (true) {
        // A. MONITOR KEYBOARD
        while (read(kbd_fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_KEY && ev.value == 1) { // Key Down
                auto now = high_resolution_clock::now();
                double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
                lastKeyTime = now;

                if (latency < 2000) { 
                    if (latency < keyStats.avgDuration * 0.3 || latency > keyStats.avgDuration * 3.0) {
                        cout << "\033[31m[!] Abnormal Typing Speed: " << latency << "ms\033[0m" << endl;
                        anomalyScore += 2;
                        
                        // --- INTEGRATION: Log Anomaly ---
                        anomalyLog.addTask("KeyAnomaly_" + to_string((int)latency) + "ms");
                        
                        // --- INTEGRATION: Check Pointer validity (Simulation) ---
                        void* ptr = PointerUtils::allocate(sizeof(int), "AnomalyFlag");
                        if(PointerUtils::isValidPointer(ptr)) {
                            PointerUtils::deallocate(ptr, "AnomalyFlag");
                        }
                    } else {
                        cout << "\033[32m[OK] Key match\033[0m" << endl;
                        if (anomalyScore > 0) anomalyScore--;
                    }
                }
            }
        }

        // B. MONITOR MOUSE
        while (read(mouse_fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_REL) {
                 double movement = abs(ev.value);
                 if (movement > mouseStats.avgDuration * 5.0) {
                     cout << "\033[31m[!] Abnormal Mouse Jerk: " << movement << "\033[0m" << endl;
                     anomalyScore += 1;
                     
                     // --- INTEGRATION: Log Anomaly ---
                     anomalyLog.addTask("MouseAnomaly_" + to_string((int)movement));
                 }
            }
        }

        // C. CHECK SCORE
        if (anomalyScore > 0) {
            cout << "Threat Level: " << anomalyScore << " / " << ANOMALY_LIMIT << endl;
            
            // --- INTEGRATION: State Transition ---
            if (currentState == "Safe") {
                if (sysState.isValidTransition("Safe", "Anomaly")) {
                    currentState = "Anomaly";
                    cout << "[Graph] System transitioned to ANOMALY state." << endl;
                }
            }
        } else {
             currentState = "Safe";
        }

        if (anomalyScore >= ANOMALY_LIMIT) {
            // --- INTEGRATION: Dump Logs before shutdown ---
            cout << "Dumping Anomaly Logs:" << endl;
            anomalyLog.showTaskList();
            
            if (sysState.isValidTransition(currentState, "Lockdown")) {
                triggerLockdown();
            }
        }

        usleep(1000);
    }

    close(kbd_fd);
    close(mouse_fd);
    return 0;
}