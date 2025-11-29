#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include "avl_profile.h"

using namespace std;
using namespace std::chrono;

const int SHUTDOWN_LIMIT = 15; // Shutdown after 15 strikes

void setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void triggerShutdown() {
    setRawMode(false);
    cout << "\n\033[1;41m[!!!] INTRUDER DETECTED - TYPING PATTERN MISMATCH [!!!]\033[0m" << endl;
    system("sudo shutdown -h now");
    exit(1);
}

int main() {
    AVLProfile validProfile;
    if (!validProfile.importFromCSV("bio_fingerprints.csv")) {
        cerr << "Error: Run bio_ssh_trainer first!" << endl;
        return 1;
    }

    UserActionProfile stats;
    if (!validProfile.search("Typing_Speed", stats)) {
        cerr << "Error: No typing data found in file." << endl;
        return 1;
    }

    cout << "--- LIVE BIOMETRIC GUARD (Active) ---" << endl;
    cout << "Authorized Avg Latency: " << stats.avgDuration << "ms" << endl;
    cout << "Type anything. Press ESC to exit safely." << endl;

    setRawMode(true);

    char c;
    auto lastKeyTime = high_resolution_clock::now();
    int anomalyScore = 0;

    while (true) {
        c = getchar();
        if (c == 27) break; // ESC to exit

        auto now = high_resolution_clock::now();
        double latency = duration_cast<milliseconds>(now - lastKeyTime).count();
        lastKeyTime = now;

        if (latency < 2000) {
            // Check deviation (If you type 2.5x slower or faster than normal)
            if (latency < stats.avgDuration * 0.4 || latency > stats.avgDuration * 2.5) {
                anomalyScore += 2;
                cout << "\033[31m[!] Anomaly (" << latency << "ms)\033[0m   \r" << endl;
            } else {
                if (anomalyScore > 0) anomalyScore--;
                cout << "\033[32m[OK] Verified\033[0m              \r" << endl;
            }
        }

        if (anomalyScore >= SHUTDOWN_LIMIT) {
            triggerShutdown();
        }
    }

    setRawMode(false);
    return 0;
}