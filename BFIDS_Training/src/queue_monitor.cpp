#include "queue_monitor.h"
#include <iostream>
using namespace std;

/*
    queue_monitor.cpp
    -----------------
    Implements a Queue data structure using a Singly Linked List.
    
    This class adheres to the First-In-First-Out (FIFO) principle:
    - New actions are Enqueued at the Rear.
    - Old actions are Dequeued from the Front.
    
    Used in the BFIDS system to buffer incoming events for sequential processing.
*/

/*
    enqueueAction
    -------------
    Adds a new UserAction to the rear of the queue.
    If the queue is empty, both frontNode and rearNode point to the new node.
*/
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

/*
    dequeueAction
    -------------
    Removes the oldest UserAction from the front of the queue.
    Updates the frontNode pointer to the next node in line.
    If the queue becomes empty, rearNode is also reset to nullptr.
*/
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

/*
    peekFront
    ---------
    Displays the details of the action currently at the front of the queue.
    Does NOT remove the element.
*/
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

/*
    clearQueue
    ----------
    Iteratively removes all nodes from the queue.
    Ensures all dynamic memory is properly deallocated.
*/
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

/*
    copyTo
    ------
    Performs a deep copy of the current queue into a target QueueMonitor object.
    
    This ensures that the target queue has its own independent copies of the nodes,
    preventing issues where modifying one queue affects the other.
*/
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