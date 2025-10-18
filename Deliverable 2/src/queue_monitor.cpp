 #include "queue_monitor.h"
#include <iostream>
using namespace std;

// Linked list node for internal queue tracking
struct Node {
    UserAction data;
    Node* next;
    Node(const UserAction& action) : data(action), next(nullptr) {}
};

// Static pointers to manage the linked list queue
static Node* frontNode = nullptr;
static Node* rearNode = nullptr;

// Enqueue a new user action (linked list implementation)
void QueueMonitor::enqueueAction(const UserAction& action) {
    Node* newNode = new Node(action);
    if (rearNode == nullptr) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    cout << "Action Enqueued (LL): " << action.action << endl;
}

// Dequeue the oldest action
void QueueMonitor::dequeueAction() {
    if (frontNode == nullptr) {
        cout << "Queue is empty. No action to dequeue." << endl;
        return;
    }
    cout << "Action Dequeued (LL): " << frontNode->data.action << endl;
    Node* temp = frontNode;
    frontNode = frontNode->next;
    if (frontNode == nullptr)
        rearNode = nullptr;
    delete temp;
}

// Peek at the front action
void QueueMonitor::peekFront() const {
    if (frontNode == nullptr) {
        cout << "Queue is empty." << endl;
    } else {
        const UserAction& front = frontNode->data;
        cout << "Front Action -> User: " << front.userID
             << ", Action: " << front.action
             << ", Status: " << front.status << endl;
    }
}

// Check if empty
bool QueueMonitor::isEmpty() const {
    return frontNode == nullptr;
}

// Get size of the queue
size_t QueueMonitor::getSize() const {
    size_t count = 0;
    Node* temp = frontNode;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}
