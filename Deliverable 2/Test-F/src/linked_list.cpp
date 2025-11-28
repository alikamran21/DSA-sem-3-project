#include "../include/linked_list.h"

LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor to free all nodes
LinkedList::~LinkedList() {
    ListNode* temp = head;
    while (temp) {
        ListNode* next = temp->next;
        delete temp;
        temp = next;
    }
    head = tail = nullptr;
}

// Insert new action at end
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

// Insert new action at beginning
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

// Delete node from end
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

// Delete node from beginning
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

// Display list from head → tail
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

// Display list from tail → head
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

// Check if list is empty
bool LinkedList::isEmpty() const {
    return head == nullptr;
}

// Get list size
size_t LinkedList::getSize() const {
    return size;
}

// Convert to circular linked list
void LinkedList::makeCircular() {
    if (head && tail) {
        tail->next = head;
        head->prev = tail;
        cout << "List converted to circular form.\n";
    }
}

// Traverse circular linked list
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

void LinkedList::copyToArray(UserAction* arr) const {
    ListNode* temp = head;
    size_t i = 0;

    while (temp != nullptr) {
        arr[i] = temp->data;
        temp = temp->next;
        i++;
    }
}