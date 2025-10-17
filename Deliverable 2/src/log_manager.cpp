#include "../include/log_manager.h"
#include <iostream>
using namespace std;

// Constructor: links monitors and file handler
LogManager::LogManager(StackMonitor* sm, QueueMonitor* qm, FileIO* fio)
    : stackMonitor(sm), queueMonitor(qm), fileIO(fio) {}

// Save all queued actions to file
void LogManager::saveLogs(const string& filename) {
    cout << "Saving logs to: " << filename << endl;
    if (fileIO && queueMonitor) {
        fileIO->writeActionsToFile(filename, queueMonitor->getAllActions());
    } else {
        cerr << "Error: Missing FileIO or QueueMonitor instance.\n";
    }
}

// Load logs from file into queue
void LogManager::loadLogs(const string& filename) {
    cout << "Loading logs from: " << filename << endl;
    if (fileIO && queueMonitor) {
        auto actions = fileIO->readActionsFromFile(filename);
        for (auto& act : actions)
            queueMonitor->enqueue(act);
    } else {
        cerr << "Error: Missing FileIO or QueueMonitor instance.\n";
    }
}
