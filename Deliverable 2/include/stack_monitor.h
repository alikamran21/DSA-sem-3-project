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

// StackMonitor class implements stack operations using linked list
class StackMonitor {
private:
    StackNode* topNode;  // Pointer to the top node
    int size;            // Number of actions in the stack

public:
    // Constructor initializes an empty stack
    StackMonitor() : topNode(nullptr), size(0) {}

    // Destructor clears the stack to free memory
    ~StackMonitor() { clearStack(); }

    // Push a new user action onto the stack
    void pushAction(const UserAction& action) {
        StackNode* newNode = new StackNode(action);
        newNode->next = topNode;
        topNode = newNode;
        size++;
        cout << "Action Pushed: " << action.action << endl;
    }

    // Pop the most recent user action from the stack
    void popAction() {
        if (isEmpty()) {
            cout << "Stack is empty. No action to pop." << endl;
            return;
        }
        StackNode* temp = topNode;
        cout << "Action Popped: " << topNode->data.action << endl;
        topNode = topNode->next;
        delete temp;
        size--;
    }

    // Display the most recent (top) user action
    void peekAction() const {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return;
        }
        const UserAction& top = topNode->data;
        cout << "Top Action -> User: " << top.userID
             << ", Action: " << top.action
             << ", Status: " << top.status << endl;
    }

    // Display all actions currently stored in the stack
    void displayAll() const {
        StackNode* curr = topNode;
        cout << "\n--- Stack Actions (Top to Bottom) ---\n";
        while (curr != nullptr) {
            cout << curr->data.userID << " -> " << curr->data.action << endl;
            curr = curr->next;
        }
    }

    // Reverse the order of elements in the stack
    void reverseStack() {
        StackNode *prev = nullptr, *curr = topNode, *next = nullptr;
        while (curr != nullptr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        topNode = prev;
        cout << "Stack reversed successfully.\n";
    }

    // Clear all elements from the stack
    void clearStack() {
        while (!isEmpty()) {
            popAction();
        }
    }

    // Check if the stack is empty
    bool isEmpty() const { return topNode == nullptr; }

    // Return the current size of the stack
    size_t getSize() const { return size; }
};

#endif
