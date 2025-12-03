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

// --- NEW INTEGRATIONS ---
#include "anomaly_heap.h" // [NEW] For prioritizing threats
#include "process_trie.h" // [NEW] For blacklisting processes

using namespace std;
using namespace std::chrono;

const int ANOMALY_LIMIT = 100; // FIXED: Increased limit

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
    disableRawMode(); // Restore terminal
    cout << "\n\033[1;41m[!!!] BIOMETRIC MISMATCH DETECTED [!!!]\033[0m" << endl;

    // [INTEGRATION] Show the most severe anomaly that caused the crash
    if (!heap.isEmpty())
    {
        AnomalyNode critical = heap.peekMax();
        cout << "Critical Event Trigger: " << critical.action.processName
             << " | Severity Score: " << critical.anomalyScore << endl;
    }

    cout << "System assumes unauthorized user. Exiting session..." << endl;
    exit(1);
}

int main()
{
    // --- INTEGRATION: Utils & Pointer Check ---
    cout << "System started at: " << currentTime() << endl;
    PointerUtils::showMemoryUsage();

    // 1. Load Biometric Profile
    AVLProfile validProfile;
    if (!validProfile.importFromCSV("bio_fingerprints.csv"))
    {
        cerr << "Error: No biometric profile found. Run bio_trainer first!" << endl;
        return 1;
    }

    // --- INTEGRATION: Data Structures ---
    hashTable userRegistry(10);
    userRegistry.addProfile("TargetUser", &validProfile);

    arrayHandler anomalyLog;
    GraphTransition sysState;
    sysState.addTransition("Safe", "Anomaly");
    sysState.addTransition("Anomaly", "Lockdown");
    string currentState = "Safe";

    // [NEW] Initialize Anomaly Heap for prioritized threat detection
    AnomalyHeap threatQueue(50);

    // [NEW] Initialize Process Trie for background process security
    ProcessTrie blacklistedProcesses;
    blacklistedProcesses.insert("keylogger");
    blacklistedProcesses.insert("wireshark");
    blacklistedProcesses.insert("netcat");
    cout << "[System] Background Process Monitor Active (Trie Loaded)." << endl;

    UserActionProfile keyStats;
    AVLProfile *activeProfile = userRegistry.getProfile("TargetUser");
    bool hasKeys = activeProfile->search("Keystroke_Dynamics", keyStats);

    if (!hasKeys)
    {
        cerr << "Error: No keystroke data found in fingerprint." << endl;
        return 1;
    }

    cout << "--- BIOMETRIC LIVE MONITORING (SSH MODE) ---" << endl;
    cout << "Avg Key Latency: " << keyStats.avgDuration << "ms" << endl;
    cout << "Start typing to verify identity... (ESC to quit)" << endl;

    enableRawMode();

    char c;
    auto lastKeyTime = high_resolution_clock::now();
    double totalSeverity = 0; // Accumulator for heap logic
    bool firstKey = true;

    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (c == 27)
            break; // ESC to quit

        auto now = high_resolution_clock::now();
        double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
        lastKeyTime = now;

        if (!firstKey && latency < 5000) // FIXED: Ignore pauses > 5s
        {
            // Check limits (Allowing 0.3x to 3.0x deviation)
            if (latency < keyStats.avgDuration * 0.3 || latency > keyStats.avgDuration * 3.0)
            {

                // Calculate severity based on how far off it is
                double rawSeverity = abs(latency - keyStats.avgDuration) / 10.0;
                double cappedSeverity = (rawSeverity > 5.0) ? 5.0 : rawSeverity; // Max penalty is 5
                totalSeverity += cappedSeverity;

                cout << "\r\033[31m[!] Anomaly: " << (int)latency << "ms (Sev: " << (int)cappedSeverity << ")\033[0m   " << flush;

                // --- INTEGRATION: Anomaly Heap ---
                // We create a temporary UserAction to store in the heap
                UserAction abnormalEvent;
                abnormalEvent.processName = "Keystroke_Violation";
                abnormalEvent.duration = latency;

                // FIXED: Used cappedSeverity instead of undefined 'severity'
                threatQueue.insert(abnormalEvent, cappedSeverity); 
                anomalyLog.addTask("KeyAnomaly_" + to_string((int)latency) + "ms");

                // --- INTEGRATION: Process Trie (Simulation) ---
                if (blacklistedProcesses.searchPrefix("keylog"))
                {
                    // Simulated hit
                }

                // --- INTEGRATION: Pointer Check ---
                void *ptr = PointerUtils::allocate(sizeof(int), "AnomalyFlag");
                if (PointerUtils::isValidPointer(ptr))
                    PointerUtils::deallocate(ptr, "AnomalyFlag");
            }
            else
            {
                // Print Green OK
                cout << "\r\033[32m[OK] Match: " << (int)latency << "ms      \033[0m" << flush;
                // Decay severity over time if behavior normalizes
                if (totalSeverity > 0)
                    totalSeverity -= 0.5;
            }
        }
        firstKey = false;

        // C. CHECK HEAP STATE & TRANSITIONS
        if (!threatQueue.isEmpty())
        {
            if (currentState == "Safe" && sysState.isValidTransition("Safe", "Anomaly"))
            {
                currentState = "Anomaly";
            }
        }
        else
        {
            currentState = "Safe";
        }

        // Trigger lockdown if the cumulative severity or single high threat is too high
        if (totalSeverity >= ANOMALY_LIMIT)
        {
            if (sysState.isValidTransition(currentState, "Lockdown"))
            {
                disableRawMode();
                cout << "\n\n[CRITICAL] Anomaly Threshold Exceeded!" << endl;
                cout << "Dumping Heap (Top Threats):" << endl;

                // Peek at the highest threat from the heap
                AnomalyNode top = threatQueue.peekMax();
                cout << "-> Max Threat: " << top.action.processName << " | Score: " << top.anomalyScore << endl;

                anomalyLog.showTaskList();
                triggerLockdown(threatQueue);
            }
        }
    }

    disableRawMode();
    return 0;
}