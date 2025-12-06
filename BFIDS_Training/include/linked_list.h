#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include "user_action.h"
using namespace std;

/*
 * LinkedList
 * ----------
 * Doubly Linked List for managing user session history.
 * Provides:
 * - insertAtEnd(): adds action to the tail
 * - insertAtBeginning(): adds action to the head
 * - deleteFromEnd(): removes action from the tail
 * - displayForward/Backward(): prints list contents
 * - makeCircular(): converts list to circular structure
 *
 * Node stores:
 * ListNode { UserAction, next, prev }
 */

// Node for Doubly Linked List
struct ListNode {
    UserAction data;
    ListNode* next;
    ListNode* prev;

    ListNode(const UserAction& action)
        : data(action), next(nullptr), prev(nullptr) {}
};

// LinkedList class for managing user session history
class LinkedList {
private:
    ListNode* head;
    ListNode* tail;
    size_t size;

public:
    LinkedList();                            // Constructor
    ~LinkedList();                           // Destructor
    void insertAtEnd(const UserAction& action);
    void insertAtBeginning(const UserAction& action);
    void deleteFromEnd();
    void deleteFromBeginning();
    void displayForward() const;
    void displayBackward() const;
    bool isEmpty() const;
    size_t getSize() const;
    void copyToArray(UserAction* arr) const;
    // Circular Linked List demo
    void makeCircular();
    void traverseCircular(size_t cycles = 1) const;
};

#endif