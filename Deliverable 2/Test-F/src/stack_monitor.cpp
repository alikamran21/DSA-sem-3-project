#include "../include/stack_monitor.h"
#include <iostream>
// NOTE: Removed includes for <stack>
using namespace std;

// Push new user action (LIFO)
void StackMonitor::pushAction(const UserAction& action) {
    StackNode* newNode = new StackNode(action);
    newNode->next = topNode;
    topNode = newNode;
    size++;
    cout << "[PUSH] Added: " << action.action << endl;
}

// Pop most recent action
void StackMonitor::popAction() {
    if (isEmpty()) {
        cout << "[POP] Stack empty. No action to pop." << endl;
        return;
    }
    StackNode* temp = topNode;
    cout << "[POP] Removed: " << topNode->data.action << endl;
    topNode = topNode->next;
    delete temp;
    size--;
}
// Peek at top action
void StackMonitor::peekAction() const {
    if (isEmpty()) {
        cout << "[PEEK] Stack empty." << endl;
        return;
    }
    const UserAction& top = topNode->data;
    cout << "[PEEK] User: " << top.userID
         << " | Action: " << top.action
         << " | Status: " << top.status << endl;
}

// Display all stack contents (non-destructive)
void StackMonitor::displayAll() const {
    if (isEmpty()) {
        cout << "[DISPLAY] Stack empty.\n";
        return;
    }
    cout << "\n[DISPLAY] Stack (Top → Bottom):\n";
    StackNode* curr = topNode;
    while (curr) {
        const UserAction& act = curr->data;
        cout << "User: " << act.userID
             << " | Action: " << act.action
             << " | Status: " << act.status << endl;
        curr = curr->next;
    }
    cout << endl;
}

// Clear all actions (proper memory deallocation)
void StackMonitor::clearStack() {
    StackNode* curr = topNode;
    while (curr) {
        StackNode* next = curr->next;
        delete curr;
        curr = next;
    }
    topNode = nullptr;
    size = 0;
    cout << "[CLEAR] Stack cleared.\n";
}

// Reverse stack order (Standard linked list reversal)
void StackMonitor::reverseStack() {
    StackNode *prev = nullptr, *curr = topNode, *next = nullptr;
    while (curr != nullptr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    topNode = prev;
    cout << "[REVERSE] Stack reversed.\n";
}

// Copy stack to another StackMonitor (Deep Copy for Linked List)
void StackMonitor::copyTo(StackMonitor& target) const {
    if (!target.isEmpty()) {
        target.clearStack(); // Ensure target is clean
    }

    // Use a temporary stack/list to hold elements in correct sequence for re-copying
    StackMonitor tempStack;
    StackNode* curr = topNode;
    while (curr != nullptr) {
        // Push all elements to temporary stack
        tempStack.pushAction(curr->data);
        curr = curr->next;
    }

    // Now copy from temporary stack to target, which reverses the order back
    StackNode* tempCurr = tempStack.topNode;
    while (tempCurr != nullptr) {
        target.pushAction(tempCurr->data);
        tempCurr = tempCurr->next;
    }

    // NOTE: tempStack's destructor handles cleanup
    cout << "[COPY] Stack copied (deep copy) to target monitor.\n";
}