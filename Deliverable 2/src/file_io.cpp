#include "../include/file_io.h"
#include <iomanip>
#include <queue>
#include <stack>
#include <sstream>
#include <algorithm>

using namespace std;

// Linked List Node for UserAction
struct Node {
    UserAction data;
    Node* next;
    Node(const UserAction& action) : data(action), next(nullptr) {}
};

// Append new node at end
void appendNode(Node*& head, const UserAction& action) {
    Node* newNode = new Node(action);
    if (!head) { head = newNode; return; }
    Node* temp = head;
    while (temp->next)
        temp = temp->next;
    temp->next = newNode;
}

// Convert linked list to array for sorting
int linkedListToArray(Node* head, UserAction arr[]) {
    int count = 0;
    Node* current = head;
    while (current) {
        arr[count++] = current->data;
        current = current->next;
    }
    return count;
}

// Helper function to compare timestamps
bool compareByTimestamp(const UserAction& a, const UserAction& b) {
    return a.timestamp < b.timestamp;
}

// Helper function to compare userIDs
bool compareByUserID(const UserAction& a, const UserAction& b) {
    return a.userID < b.userID;
}

// Save all actions (formatted table + sorted)
bool FileIO::saveActionsToFile(Node* head, const string& filename) {
    if (!head) {
        cerr << "Error: No actions to save!\n";
        return false;
    }

    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return false;
    }

    // Convert linked list to array for sorting
    UserAction actions[100]; // static array (no vector)
    int count = linkedListToArray(head, actions);

    // Ask user how to sort
    int choice;
    cout << "\nSort logs by:\n1. Timestamp\n2. User ID\nEnter choice: ";
    cin >> choice;

    if (choice == 2)
        sort(actions, actions + count, compareByUserID);
    else
        sort(actions, actions + count, compareByTimestamp);

    // Queue and stack demonstration
    queue<UserAction> fifo;
    stack<UserAction> lifo;
    for (int i = 0; i < count; i++) {
        fifo.push(actions[i]);
        lifo.push(actions[i]);
    }

    // Write formatted table header
    file << left << setw(25) << "Timestamp"
         << setw(10) << "UserID"
         << setw(20) << "Action"
         << setw(20) << "Process"
         << setw(12) << "Duration"
         << setw(18) << "Next_Action"
         << setw(10) << "Status" << endl;
    file << string(120, '-') << endl;

    // Write actions (FIFO order)
    while (!fifo.empty()) {
        UserAction act = fifo.front();
        fifo.pop();

        file << left << setw(25) << act.timestamp
             << setw(10) << act.userID
             << setw(20) << act.action
             << setw(20) << act.processName
             << setw(12) << act.duration
             << setw(18) << act.nextAction
             << setw(10) << act.status << endl;
    }

    file << string(120, '=') << "\n\n";
    file.close();

    cout << "\n✅ Actions saved to file '" << filename << "' in table format and sorted.\n";
    return true;
}

// Read file and print to console (formatted)
void FileIO::readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    cout << "\n File Contents:\n";
    while (getline(file, line))
        cout << line << endl;
    file.close();
}
