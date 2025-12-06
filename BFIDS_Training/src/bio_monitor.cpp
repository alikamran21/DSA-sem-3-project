#include <iostream>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <termios.h>
#include <string>

// --- INCLUDES ---
#include "avl_profile.h"
#include "Array_handler.h"
#include "pointer_utils.h"
#include "utils.h"
#include "hash_profiles.h"
#include "graph_transition.h"
#include "anomaly_heap.h" 
#include "process_trie.h" 

using namespace std;
using namespace std::chrono;

/*
    bio_monitor.cpp
    ---------------
    The core Live Detection Engine for the BFIDS system (SSH Mode).
    
    Functionality:
    1. Loads a trained biometric fingerprint for a specific user.
    2. Initializes the Security State Graph (Safe -> Warning -> Anomaly -> Lockdown).
    3. Monitors real-time keystroke latency via SSH/Terminal.
    4. Calculates anomaly scores based on deviation from the baseline.
    5. Triggers an emergency lockdown if the cumulative threat score exceeds a limit.

    DSA Concepts:
    - AVL Tree (Profile Search)
    - Heap (Priority Queue for Anomalies)
    - Graph (State Machine)
    - Trie (Process Blacklisting)
*/

const int ANOMALY_LIMIT = 50;

// Terminal handling
struct termios orig_termios;

/*
    disableRawMode
    --------------
    Restores the terminal to its original canonical mode.
    Ensures the terminal behaves normally after the program exits.
*/
void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

/*
    enableRawMode
    -------------
    Configures the terminal to read input byte-by-byte (Raw Mode).
    Disables local echo so keystrokes are captured but not necessarily printed immediately.
    Essential for capturing precise timing of individual key presses.
*/
void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/*
    triggerLockdown
    ---------------
    Executes the emergency response protocol.
    1. Restores terminal settings.
    2. Displays the critical anomaly event from the Heap.
    3. Executes system commands to sync disk and shut down.
*/
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

/*
    main
    ----
    Entry point for the monitoring system.
    Orchestrates the loading of profiles, initialization of structures (Heap, Graph, Trie),
    and the main event loop for keystroke analysis.
*/
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
    // --- CHANGE: Look in fingerprints folder ---
    string filename = "fingerprints/bio_fingerprints_" + targetUser + ".csv";
    AVLProfile validProfile;
    
    // Attempt to load the user's behavioral fingerprint from disk
    if (!validProfile.importFromCSV(filename))
    {
        cerr << "Error: No biometric profile found for user '" << targetUser << "'." << endl;
        cerr << "Please run bio_trainer and train as '" << targetUser << "' first." << endl;
        return 1;
    }

    // Register profile in the Hash Table
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

    // Initialize Anomaly Detection Structures
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

    // Main Monitoring Loop
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (c == 27) break; // Exit on ESC

        auto now = high_resolution_clock::now();
        double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
        lastKeyTime = now;

        if (!firstKey && latency < 5000) 
        {
            // Check for deviations (30% - 300% of average)
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

                // Temporary memory allocation check (simulation of memory tracking)
                void *ptr = PointerUtils::allocate(sizeof(int), "AnomalyFlag");
                if (PointerUtils::isValidPointer(ptr))
                    PointerUtils::deallocate(ptr, "AnomalyFlag");
            }
            else
            {
                // Normal behavior: cool down severity
                cout << "\r\033[32m[OK] Match: " << (int)latency << "ms      \033[0m" << flush;
                if (totalSeverity > 0) totalSeverity -= 0.5;
            }
        }
        firstKey = false;

        // --- GRAPH STATE LOGIC ---
        // Transitions state based on threat presence
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

        // Trigger Lockdown if threshold exceeded
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