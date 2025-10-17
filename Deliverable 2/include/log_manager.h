#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <iostream>
#include <string>
#include "user_action.h"
#include "stack_monitor.h"
#include "queue_monitor.h"
#include "file_io.h"

using namespace std;

class LogManager {
private:
    StackMonitor stackMonitor;
    QueueMonitor queueMonitor;
    FileIO fileHandler;

public:
    void recordAction(const string& user, const string& desc) {
        UserAction action(user, desc);
        stackMonitor.pushAction(action);
        queueMonitor.enqueueAction(action);
        fileHandler.writeAction(action);
        cout << "Action recorded successfully.\n";
    }

    void undoLastAction() {
        if (stackMonitor.isEmpty()) {
            cout << "No action to undo.\n";
            return;
        }
        UserAction undone = stackMonitor.popAction();
        cout << "Undone action: " << undone.description << endl;
    }

    void showActionHistory() {
        cout << "\n--- Recent Actions (Queue) ---\n";
        queueMonitor.displayQueue();
    }

    void loadPreviousLogs() {
        fileHandler.readAllActions();
    }
};

#endif
