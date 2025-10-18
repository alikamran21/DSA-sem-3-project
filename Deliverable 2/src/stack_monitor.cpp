#include "../include/stack_monitor.h"
#include <iostream>
using namespace std;

void StackMonitor::pushAction(const UserAction& action) {
    actionStack.push(action);
    cout << "Action pushed to stack: " << action.action << endl;
}

void StackMonitor::popAction() {
    if (!actionStack.empty()) {
        cout << "Popped action: " << actionStack.top().action << endl;
        actionStack.pop();
    } else {
        cout << "Stack is empty.\n";
    }
}

void StackMonitor::peekAction() const {
    if (!actionStack.empty()) {
        const UserAction& top = actionStack.top();
        cout << "Top -> User: " << top.userID
             << ", Action: " << top.action
             << ", Status: " << top.status << endl;
    } else {
        cout << "Stack empty.\n";
    }
}

bool StackMonitor::isEmpty() const {
    return actionStack.empty();
}

size_t StackMonitor::getSize() const {
    return actionStack.size();
}
