#include <iostream>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <termios.h>
#include <string>

// --- EXISTING INCLUDES ---
#include "avl_profile.h"
#include "Array_handler.h"
#include "pointer_utils.h"
#include "utils.h"
#include "hash_profiles.h"
#include "graph_transition.h"

// --- NEW INTEGRATIONS ---
#include "anomaly_heap.h" 
#include "process_trie.h" 

using namespace std;
using namespace std::chrono;

const int ANOMALY_LIMIT = 50;

// Terminal handling
struct termios orig_termios;
void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }
void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void triggerLockdown(AnomalyHeap &heap)
{
    disableRawMode(); // Restore terminal settings first

    cout << "\n\033[1;41m[!!!] BIOMETRIC MISMATCH DETECTED [!!!]\033[0m" << endl;
    cout << "\033[1;31m[CRITICAL] UNAUTHORIZED USER DETECTED.\033[0m" << endl;

    // Show the specific anomaly that caused the trigger
    if (!heap.isEmpty())
    {
        AnomalyNode critical = heap.peekMax();
        cout << "Trigger Event: " << critical.action.processName
             << " | Severity: " << critical.anomalyScore << endl;
    }

    cout << "Initiating Emergency Shutdown..." << endl;

    // --- CRITICAL CHANGE ---
    // 'sync' forces a write of all data to disk (prevents corruption)
    // 'shutdown now' turns off the machine immediately
    system("sync; sudo shutdown now"); 
}

int main()
{
    cout << "System started at: " << currentTime() << endl;
    PointerUtils::showMemoryUsage();

    // --- 1. ASK FOR USER IDENTITY ---
    string targetUser;
    cout << "Enter username to monitor: ";
    cin >> targetUser;
    cin.ignore(); 

    // --- 2. LOAD PROFILE ---
    string filename = "bio_fingerprints_" + targetUser + ".csv";
    AVLProfile validProfile;
    
    if (!validProfile.importFromCSV(filename))
    {
        cerr << "Error: No biometric profile found for user '" << targetUser << "'." << endl;
        cerr << "Please run bio_trainer and train as '" << targetUser << "' first." << endl;
        return 1;
    }

    hashTable userRegistry(10);
    userRegistry.addProfile(targetUser, &validProfile);

    // --- 3. GRAPH STATE MACHINE (UPDATED) ---
    arrayHandler anomalyLog;
    GraphTransition sysState;
    
    // Define Security States
    sysState.addTransition("Safe", "Warning");
    sysState.addTransition("Warning", "Anomaly");
    sysState.addTransition("Anomaly", "Lockdown");
    sysState.addTransition("Warning", "Safe"); // Recovery path
    
    string currentState = "Safe";

    cout << "\n[System] Verifying Security State Graph Integrity..." << endl;
    sysState.bfs("Safe"); // Verify reachability using BFS
    sysState.dfs("Safe"); // Verify paths using DFS
    cout << "[System] Graph verified.\n" << endl;

    AnomalyHeap threatQueue(50);
    ProcessTrie blacklistedProcesses;
    blacklistedProcesses.insert("keylogger");
    blacklistedProcesses.insert("wireshark");

    UserActionProfile keyStats;
    AVLProfile *activeProfile = userRegistry.getProfile(targetUser);
    
    if (!activeProfile->search("Keystroke_Dynamics", keyStats))
    {
        cerr << "Error: No keystroke data found in fingerprint for " << targetUser << "." << endl;
        return 1;
    }

    cout << "--- BIOMETRIC LIVE MONITORING (SSH MODE) ---" << endl;
    cout << "Monitoring User: " << targetUser << endl;
    cout << "Avg Key Latency: " << keyStats.avgDuration << "ms" << endl;
    cout << "Start typing to verify identity... (ESC to quit)" << endl;

    enableRawMode();

    char c;
    auto lastKeyTime = high_resolution_clock::now();
    double totalSeverity = 0; 
    bool firstKey = true;

    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (c == 27) break; 

        auto now = high_resolution_clock::now();
        double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
        lastKeyTime = now;

        if (!firstKey && latency < 5000) 
        {
            if (latency < keyStats.avgDuration * 0.3 || latency > keyStats.avgDuration * 3.0)
            {
                double rawSeverity = abs(latency - keyStats.avgDuration) / 10.0;
                double cappedSeverity = (rawSeverity > 5.0) ? 5.0 : rawSeverity;
                totalSeverity += cappedSeverity;

                cout << "\r\033[31m[!] Anomaly: " << (int)latency << "ms (Sev: " << (int)cappedSeverity << ")\033[0m   " << flush;

                UserAction abnormalEvent;
                abnormalEvent.userID = targetUser;
                abnormalEvent.processName = "Keystroke_Violation";
                abnormalEvent.duration = latency;

                threatQueue.insert(abnormalEvent, cappedSeverity); 
                anomalyLog.addTask("KeyAnomaly_" + to_string((int)latency) + "ms");

                void *ptr = PointerUtils::allocate(sizeof(int), "AnomalyFlag");
                if (PointerUtils::isValidPointer(ptr))
                    PointerUtils::deallocate(ptr, "AnomalyFlag");
            }
            else
            {
                cout << "\r\033[32m[OK] Match: " << (int)latency << "ms      \033[0m" << flush;
                if (totalSeverity > 0) totalSeverity -= 0.5;
            }
        }
        firstKey = false;

        // --- GRAPH STATE LOGIC ---
        // If threats exist, transition Safe -> Warning -> Anomaly
        if (!threatQueue.isEmpty())
        {
            if (currentState == "Safe" && sysState.isValidTransition("Safe", "Warning"))
                currentState = "Warning";
            else if (currentState == "Warning" && sysState.isValidTransition("Warning", "Anomaly"))
                currentState = "Anomaly";
        }
        else
        {
             // Recovery logic
             if (currentState == "Warning" && sysState.isValidTransition("Warning", "Safe"))
                currentState = "Safe";
        }

        if (totalSeverity >= ANOMALY_LIMIT)
        {
            if (sysState.isValidTransition(currentState, "Lockdown") || currentState == "Anomaly")
            {
                disableRawMode();
                cout << "\n\n[CRITICAL] Anomaly Threshold Exceeded for user " << targetUser << "!" << endl;
                cout << "State Transitioned: " << currentState << " -> Lockdown" << endl;
                triggerLockdown(threatQueue);
            }
        }
    }

    disableRawMode();
    return 0;
}