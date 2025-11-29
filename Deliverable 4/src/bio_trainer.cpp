#include <iostream>
#include <vector>
#include <termios.h> 
#include <unistd.h>
#include <chrono>
#include "avl_profile.h"

using namespace std;
using namespace std::chrono;

// FUNCTION: Set Terminal to "Raw Mode" (Reads keys instantly, no Enter needed)
void setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Disable buffering and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore normal settings
    }
}

int main() {
    AVLProfile profile;
    cout << "--- KEYSTROKE DYNAMICS TRAINER (SSH COMPATIBLE) ---" << endl;
    cout << "System will learn your typing speed." << endl;
    cout << ">>> TYPE NORMALLY. Press 'ESC' (twice) to finish." << endl;

    setRawMode(true); // Enable biometrics mode

    char c;
    auto lastKeyTime = high_resolution_clock::now();
    bool running = true;

    while (running) {
        c = getchar(); // Read 1 char immediately

        // Check for ESC (ASCII 27)
        if (c == 27) { 
            running = false;
            break; 
        }

        auto now = high_resolution_clock::now();
        double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
        lastKeyTime = now;

        // Filter out long pauses (thinking time) -> Only capture typing rhythm
        if (latency < 1000) {
            // Print \r to return to start of line since we disabled standard output
            cout << "Key detected. Latency: " << latency << "ms\r" << endl;
            profile.insertOrUpdate("Typing_Speed", latency);
        }
    }

    setRawMode(false); // Restore terminal so you can see what you type again

    cout << "\nTraining Complete." << endl;
    if (profile.exportToCSV("bio_fingerprints.csv")) {
        cout << ">> Biometric Profile saved to 'bio_fingerprints.csv'" << endl;
    } else {
        cerr << ">> Error saving file!" << endl;
    }

    return 0;
}