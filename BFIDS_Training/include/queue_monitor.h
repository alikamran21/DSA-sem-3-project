#ifndef QUEUE_MONITOR_H
#define QUEUE_MONITOR_H
#include <iostream>
#include "user_action.h"
using namespace std;

/*
 * QueueMonitor
 * ------------
 * Queue implementation (FIFO) using a linked list for action processing.
 * Provides:
 * - enqueueAction(): adds an action to the rear
 * - dequeueAction(): removes an action from the front
 * - peekFront(): views the front action
 * - clearQueue(): removes all elements
 *
 * Node stores:
 * QueueNode { UserAction, next }
 */

// Node structure for linked list implementation of queue
struct QueueNode {
    UserAction data;
    QueueNode* next;
    QueueNode(const UserAction& act) : data(act), next(nullptr) {}
};

// Class to manage user actions using a queue (FIFO)
class QueueMonitor {
private:
    QueueNode* frontNode; // Head of the list (Dequeue end)
    QueueNode* rearNode;  // Tail of the list (Enqueue end)
    size_t size;          // Number of actions

public:
    // Constructor initializes an empty queue
    QueueMonitor() : frontNode(nullptr), rearNode(nullptr), size(0) {}

    // Destructor to free all nodes
    ~QueueMonitor() { clearQueue(); }

    // Core Queue Operations
    void enqueueAction(const UserAction& action);
    void dequeueAction();
    void peekFront() const;

    // Utility
    void clearQueue();
    void copyTo(QueueMonitor& target) const;

    bool isEmpty() const { return frontNode == nullptr; }
    size_t getSize() const { return size; }
};

#endif