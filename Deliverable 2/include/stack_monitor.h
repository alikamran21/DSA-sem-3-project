#ifndef STACK_MONITOR_H
#define STACK_MONITOR_H
#include <iostream>
#include <stack>
#include "user_action.h"
using namespace std;

// Class to manage user actions using a stack
class StackMonitor {
private:
    stack<UserAction> actionStack; // Stack to store user actions

public:
    // Push a new user action onto the stack
    void pushAction(const UserAction& action) {
        actionStack.push(action);
        cout << "Action Pushed: " << action.action << endl;
    }

    // Pop the most recent action
    void popAction() {
        if (!actionStack.empty()) {
            cout << "Action Popped: " << actionStack.top().action << endl;
            actionStack.pop();
        } else {
            cout << "Stack is empty. No action to pop." << endl;
        }
    }

    // View the most recent action
    void peekAction() const {
        if (!actionStack.empty()) {
            const UserAction& top = actionStack.top();
            cout << "Top Action -> User: " << top.userID
                 << ", Action: " << top.action
                 << ", Status: " << top.status << endl;
        } else {
            cout << "Stack is empty." << endl;
        }
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return actionStack.empty();
    }

    // Get stack size
    size_t getSize() const {
        return actionStack.size();
    }
};

#endif // STACK_MONITOR_H
