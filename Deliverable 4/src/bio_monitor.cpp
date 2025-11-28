#include <iostream>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <cstdlib> // For system()
#include "avl_profile.h"

using namespace std;
using namespace std::chrono;

// --- CONFIGURATION ---
const char* KEYBOARD_DEV = "/dev/input/event2"; // MATCH TRAINER
const char* MOUSE_DEV    = "/dev/input/event3"; // MATCH TRAINER
const int ANOMALY_LIMIT  = 20;

void triggerLockdown() {
    cout << "\n\033[1;41m[!!!] BIOMETRIC MISMATCH DETECTED [!!!]\033[0m" << endl;
    cout << "System assumes unauthorized user. Shutting down..." << endl;
    system("sudo shutdown -h now");
    exit(1);
}

int main() {
    // 1. Load Biometric Profile
    AVLProfile validProfile;
    if (!validProfile.importFromCSV("bio_fingerprints.csv")) {
        cerr << "Error: No biometric profile found. Run bio_trainer first!" << endl;
        return 1;
    }
    
    // Get stats for comparison
    UserActionProfile keyStats, mouseStats;
    bool hasKeys = validProfile.search("Keystroke_Dynamics", keyStats);
    bool hasMouse = validProfile.search("Mouse_Dynamics", mouseStats);

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

                if (latency < 2000) { // Ignore idle breaks
                    // If you type 3x faster or 3x slower than your average
                    if (latency < keyStats.avgDuration * 0.3 || latency > keyStats.avgDuration * 3.0) {
                        cout << "\033[31m[!] Abnormal Typing Speed: " << latency << "ms\033[0m" << endl;
                        anomalyScore += 2;
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
                 // If mouse moves explosively fast compared to your average
                 if (movement > mouseStats.avgDuration * 5.0) {
                     cout << "\033[31m[!] Abnormal Mouse Jerk: " << movement << "\033[0m" << endl;
                     anomalyScore += 1;
                 }
            }
        }

        // C. CHECK SCORE
        if (anomalyScore > 0) 
            cout << "Threat Level: " << anomalyScore << " / " << ANOMALY_LIMIT << endl;

        if (anomalyScore >= ANOMALY_LIMIT) {
            triggerLockdown();
        }

        usleep(1000);
    }

    close(kbd_fd);
    close(mouse_fd);
    return 0;
}