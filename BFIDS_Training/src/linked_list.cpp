#include "linked_list.h"

/*
    linked_list.cpp
    ---------------
    Implements a Doubly Linked List for storing UserActions.
    
    This structure is fundamental for maintaining a session history where:
    - Insertions happen at both ends (Head/Tail) for flexibility.
    - Deletions can occur from both ends (e.g., trimming old logs).
    - Traversal can be done forwards or backwards for analysis.
    
    DSA Concepts:
    - Doubly Linked Nodes (prev/next pointers)
    - Dynamic Memory Management (new/delete)
*/

LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

/*
    ~LinkedList
    -----------
    Destructor.
    Traverses the list and frees every node to prevent memory leaks.
*/
LinkedList::~LinkedList() {
    ListNode* temp = head;
    while (temp) {
        ListNode* next = temp->next;
        delete temp;
        temp = next;
    }
    head = tail = nullptr;
}

/*
    insertAtEnd
    -----------
    Adds a new UserAction to the tail of the list.
    Complexity: O(1) due to the tail pointer.
*/
void LinkedList::insertAtEnd(const UserAction& action) {
    ListNode* newNode = new ListNode(action);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
    // cout << "Inserted at end: " << action.action << endl; 
}

/*
    insertAtBeginning
    -----------------
    Adds a new UserAction to the head of the list.
    Complexity: O(1).
*/
void LinkedList::insertAtBeginning(const UserAction& action) {
    ListNode* newNode = new ListNode(action);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    size++;
    // cout << "Inserted at beginning: " << action.action << endl;
}

/*
    deleteFromEnd
    -------------
    Removes the last node (tail) from the list.
    Updates the new tail's next pointer to nullptr.
*/
void LinkedList::deleteFromEnd() {
    if (!tail) {
        cout << "List is empty.\n";
        return;
    }
    cout << "Deleted from end: " << tail->data.action << endl;
    ListNode* temp = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr;
    else head = nullptr;
    delete temp;
    size--;
}

/*
    deleteFromBeginning
    -------------------
    Removes the first node (head) from the list.
    Updates the new head's prev pointer to nullptr.
*/
void LinkedList::deleteFromBeginning() {
    if (!head) {
        cout << "List is empty.\n";
        return;
    }
    cout << "Deleted from beginning: " << head->data.action << endl;
    ListNode* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    else tail = nullptr;
    delete temp;
    size--;
}

/*
    displayForward
    --------------
    Traverses the list from Head to Tail and prints action details.
*/
void LinkedList::displayForward() const {
    if (!head) {
        cout << "List is empty.\n";
        return;
    }
    ListNode* temp = head;
    cout << "\nSession History (Forward):\n";
    while (temp) {
        cout << temp->data.userID << " - " << temp->data.action << " - " << temp->data.status << endl;
        temp = temp->next;
    }
}

/*
    displayBackward
    ---------------
    Traverses the list from Tail to Head using 'prev' pointers.
    Useful for reviewing the most recent history first.
*/
void LinkedList::displayBackward() const {
    if (!tail) {
        cout << "List is empty.\n";
        return;
    }
    ListNode* temp = tail;
    cout << "\nSession History (Backward):\n";
    while (temp) {
        cout << temp->data.userID << " - " << temp->data.action << " - " << temp->data.status << endl;
        temp = temp->prev;
    }
}

/*
    isEmpty
    -------
    Checks if the list contains any nodes.
*/
bool LinkedList::isEmpty() const {
    return head == nullptr;
}

/*
    getSize
    -------
    Returns the current number of nodes in the list.
*/
size_t LinkedList::getSize() const {
    return size;
}

/*
    makeCircular
    ------------
    Connects the tail to the head, transforming the linear list into a circular one.
    This creates a loop where traversing past the tail brings you back to the head.
*/
void LinkedList::makeCircular() {
    if (head && tail) {
        tail->next = head;
        head->prev = tail;
        cout << "List converted to circular form.\n";
    }
}

/*
    traverseCircular
    ----------------
    Iterates through the list multiple times (specified by 'cycles').
    Demonstrates the circular property by going past the physical end of the list.
*/
void LinkedList::traverseCircular(size_t cycles) const {
    if (!head) {
        cout << "List is empty.\n";
        return;
    }

    cout << "\nCircular Traversal (" << cycles << " cycles):\n";
    ListNode* temp = head;
    size_t count = 0;

    do {
        cout << temp->data.action << " -> ";
        temp = temp->next;
        count++;
    } while (temp && count < cycles * size);

    cout << "...\n";
}

/*
    copyToArray
    -----------
    Copies all UserAction data from the linked list into a provided C-style array.
    This is often used before sorting, as arrays allow random access (needed for Quicksort/Mergesort).
*/
void LinkedList::copyToArray(UserAction* arr) const {
    ListNode* temp = head;
    size_t i = 0;

    while (temp != nullptr) {
        arr[i] = temp->data;
        temp = temp->next;
        i++;
    }
}