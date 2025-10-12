#ifndef QUEUE_MONITOR_H
#define QUEUE_MONITOR_H

#include <iostream>
#include <queue>
#include "user_action.h"
using namespace std;

// Class to manage user actions using a queue (FIFO)
class QueueMonitor {
private:
    queue<UserAction> actionQueue;

public:
    // Enqueue a new user action
    void enqueueAction(const UserAction& action) {
        actionQueue.push(action);
        cout << "Action Enqueued: " << action.action << endl;
    }

    // Dequeue the oldest action
    void dequeueAction() {
        if (!actionQueue.empty()) {
            cout << "Action Dequeued: " << actionQueue.front().action << endl;
            actionQueue.pop();
        } else {
            cout << "Queue is empty. No action to dequeue." << endl;
        }
    }

    // Peek the front action
    void peekFront() const {
        if (!actionQueue.empty()) {
            const UserAction& front = actionQueue.front();
            cout << "Front Action -> User: " << front.userID
                 << ", Action: " << front.action
                 << ", Status: " << front.status << endl;
        } else {
            cout << "Queue is empty." << endl;
        }
    }

    // Check if empty
    bool isEmpty() const {
        return actionQueue.empty();
    }

    // Get queue size
    size_t getSize() const {
        return actionQueue.size();
    }
};

#endif // QUEUE_MONITOR_H
