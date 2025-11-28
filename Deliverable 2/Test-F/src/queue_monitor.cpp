#include "../include/queue_monitor.h"
#include <iostream>
using namespace std;

// Enqueue a new user action (insert at rear)
void QueueMonitor::enqueueAction(const UserAction& action) {
    QueueNode* newNode = new QueueNode(action);
    if (rearNode == nullptr) {
        // First element
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    size++;
    cout << "[ENQUEUE] Added: " << action.action << endl;
}

// Dequeue the oldest action (delete from front)
void QueueMonitor::dequeueAction() {
    if (frontNode == nullptr) {
        cout << "[DEQUEUE] Queue is empty. No action to dequeue." << endl;
        return;
    }
    cout << "[DEQUEUE] Removed: " << frontNode->data.action << endl;
    QueueNode* temp = frontNode;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        // Last element removed
        rearNode = nullptr;
    }
    delete temp;
    size--;
}

// Peek at the front action
void QueueMonitor::peekFront() const {
    if (frontNode == nullptr) {
        cout << "[PEEK] Queue is empty." << endl;
    } else {
        const UserAction& front = frontNode->data;
        cout << "[PEEK] Front Action -> User: " << front.userID
             << ", Action: " << front.action
             << ", Status: " << front.status << endl;
    }
}

// Clear all elements (proper memory deallocation)
void QueueMonitor::clearQueue() {
    QueueNode* curr = frontNode;
    while (curr) {
        QueueNode* next = curr->next;
        delete curr;
        curr = next;
    }
    frontNode = rearNode = nullptr;
    size = 0;
    cout << "[CLEAR] Queue cleared.\n";
}

// Copy queue to another QueueMonitor (Deep Copy)
void QueueMonitor::copyTo(QueueMonitor& target) const {
    if (!target.isEmpty()) {
        target.clearQueue(); // Ensure target is clean
    }

    QueueNode* curr = frontNode;
    while (curr != nullptr) {
        // Enqueuing to the target automatically creates deep copies of the nodes
        target.enqueueAction(curr->data);
        curr = curr->next;
    }
    cout << "[COPY] Queue copied (deep copy) to target monitor.\n";
}