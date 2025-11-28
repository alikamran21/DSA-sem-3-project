#include <iostream>
#include <fstream>
#include <vector>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <chrono>
#include "../include/avl_profile.h" // Reusing your AVL Tree!

using namespace std;
using namespace std::chrono;

// --- CONFIGURATION: CHANGE THESE IDS TO MATCH YOUR VM ---
const char* KEYBOARD_DEV = "/dev/input/event2"; // CHECK YOUR ID!
const char* MOUSE_DEV    = "/dev/input/event3"; // CHECK YOUR ID!

// Helper struct for mouse movement
struct MouseState {
    int x = 0;
    int y = 0;
};

int main() {
    cout << "--- BIOMETRIC TRAINING PHASE ---" << endl;
    cout << "Opening devices (Run with sudo!)..." << endl;

    int kbd_fd = open(KEYBOARD_DEV, O_RDONLY | O_NONBLOCK);
    int mouse_fd = open(MOUSE_DEV, O_RDONLY | O_NONBLOCK);

    if (kbd_fd == -1 || mouse_fd == -1) {
        cerr << "Error: Could not open devices. Did you run with 'sudo'? Did you update event IDs?" << endl;
        return 1;
    }

    AVLProfile profile;
    struct input_event ev;
    auto lastKeyTime = high_resolution_clock::now();
    
    cout << ">>> TRAINER ACTIVE. PLEASE TYPE AND MOVE MOUSE NORMALLY." << endl;
    cout << ">>> (Press 'ESC' to finish training)" << endl;

    bool training = true;
    while (training) {
        // 1. READ KEYBOARD
        while (read(kbd_fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_KEY && ev.value == 1) { // Key Press
                auto now = high_resolution_clock::now();
                double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
                lastKeyTime = now;

                if (ev.code == KEY_ESC) {
                    training = false;
                    break;
                }

                // Ignore huge pauses (idle time)
                if (latency < 2000) { 
                    cout << "[KEY] Latency: " << latency << "ms" << endl;
                    profile.insertOrUpdate("Keystroke_Dynamics", latency);
                }
            }
        }

        // 2. READ MOUSE
        while (read(mouse_fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_REL) { // Relative movement
                // We treat mouse movement 'magnitude' as the value to profile
                // This is a simplified metric for mouse speed/jitter
                double movement = abs(ev.value);
                if (movement > 0) {
                     // cout << "[MOUSE] Move: " << movement << endl;
                     profile.insertOrUpdate("Mouse_Dynamics", movement);
                }
            }
        }
        
        usleep(1000); // Prevent CPU hogging
    }

    cout << "\nTraining Complete. Saving Profile..." << endl;
    profile.exportToCSV("bio_fingerprints.csv");
    cout << "Saved to 'bio_fingerprints.csv'." << endl;

    close(kbd_fd);
    close(mouse_fd);
    return 0;
}