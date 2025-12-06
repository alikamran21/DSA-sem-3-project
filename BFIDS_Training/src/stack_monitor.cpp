#include "stack_monitor.h"
#include <iostream>
// NOTE: Removed includes for <stack>
using namespace std;

/*
    stack_monitor.cpp
    -----------------
    Implements a Stack data structure using a Linked List (LIFO).
    Used for backtracking or undo-like operations on user actions.
*/

/*
    pushAction
    ----------
    Pushes a new action onto the top of the stack.
*/
void StackMonitor::pushAction(const UserAction& action) {
    StackNode* newNode = new StackNode(action);
    newNode->next = topNode;
    topNode = newNode;
    size++;
    cout << "[PUSH] Added: " << action.action << endl;
}

/*
    popAction
    ---------
    Removes the action from the top of the stack.
    Checks if stack is empty before popping.
*/
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

/*
    peekAction
    ----------
    Displays the top action without removing it.
*/
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

/*
    displayAll
    ----------
    Traverses the stack from Top to Bottom and displays all elements.
*/
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

/*
    clearStack
    ----------
    Removes all nodes from the stack and frees memory.
*/
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

/*
    reverseStack
    ------------
    Reverses the order of elements in the stack in-place.
    Useful for analyzing history in chronological vs reverse-chronological order.
*/
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

/*
    copyTo
    ------
    Creates a deep copy of the stack to another StackMonitor.
    Uses a temporary stack to preserve the order during copying.
*/
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