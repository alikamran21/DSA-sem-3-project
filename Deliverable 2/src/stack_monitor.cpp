#include "../include/stack_monitor.h"
#include <iostream>
using namespace std;

// Push new user action (LIFO)
void StackMonitor::pushAction(const UserAction& action) {
    actionStack.push(action);
    cout << "[PUSH] Added: " << action.action << " by " << action.userID << endl;
}

// Pop most recent action
void StackMonitor::popAction() {
    if (!actionStack.empty()) {
        cout << "[POP] Removed: " << actionStack.top().action << endl;
        actionStack.pop();
    } else {
        cout << "[POP] Stack empty.\n";
    }
}

// Peek at top action
void StackMonitor::peekAction() const {
    if (!actionStack.empty()) {
        const UserAction& top = actionStack.top();
        cout << "[PEEK] User: " << top.userID
             << " | Action: " << top.action
             << " | Status: " << top.status << endl;
    } else {
        cout << "[PEEK] Stack empty.\n";
    }
}

// Check if stack is empty
bool StackMonitor::isEmpty() const { return actionStack.empty(); }

// Return number of actions
size_t StackMonitor::getSize() const { return actionStack.size(); }

// Display all stack contents (non-destructive)
void StackMonitor::displayAll() const {
    if (actionStack.empty()) {
        cout << "[DISPLAY] Stack empty.\n";
        return;
    }
    cout << "\n[DISPLAY] Stack (Top → Bottom):\n";
    stack<UserAction> temp = actionStack;
    while (!temp.empty()) {
        const UserAction& act = temp.top();
        cout << "User: " << act.userID
             << " | Action: " << act.action
             << " | Status: " << act.status << endl;
        temp.pop();
    }
    cout << endl;
}

// Clear all actions
void StackMonitor::clearStack() {
    while (!actionStack.empty())
        actionStack.pop();
    cout << "[CLEAR] Stack cleared.\n";
}

// Reverse stack order
void StackMonitor::reverseStack() {
    stack<UserAction> reversed;
    while (!actionStack.empty()) {
        reversed.push(actionStack.top());
        actionStack.pop();
    }
    actionStack = reversed;
    cout << "[REVERSE] Stack reversed.\n";
}

// Copy stack to another StackMonitor
void StackMonitor::copyTo(StackMonitor& target) const {
    target.actionStack = actionStack;
    cout << "[COPY] Stack copied to target monitor.\n";
}
