#ifndef STACK_MONITOR_H
#define STACK_MONITOR_H

#include <iostream>
#include "user_action.h"
using namespace std;

// Node structure for linked list implementation of stack
struct StackNode {
    UserAction data;
    StackNode* next;
    StackNode(const UserAction& act) : data(act), next(nullptr) {}
};

// StackMonitor class implements stack operations using linked list (LIFO)
class StackMonitor {
private:
    StackNode* topNode;  // Pointer to the top node
    int size;            // Number of actions in the stack

public:
    // Constructor initializes an empty stack
    StackMonitor() : topNode(nullptr), size(0) {}

    // Destructor clears the stack to free memory
    ~StackMonitor() { clearStack(); }

    // Core Stack Operations
    void pushAction(const UserAction& action);
    void popAction();
    void peekAction() const;

    // Utility
    void displayAll() const;
    void reverseStack();
    void clearStack();
    void copyTo(StackMonitor& target) const; // For deep copy

    bool isEmpty() const { return topNode == nullptr; }
    int getSize() const { return size; }
};

#endif // STACK_MONITOR_H