#include "../include/file_io.h"
#include <iomanip>
#include <sstream>
#include <algorithm> // For std::sort
#include <ctime>
#include <limits>
#include <cstring>   // For strcpy

using namespace std;

// Node constructor defined here as declared in file_io.h
Node::Node(const UserAction& action) : data(action), next(nullptr) {}

// Append new node at end
void appendNode(Node*& head, const UserAction& action) {
    Node* newNode = new Node(action);
    if (!head) { head = newNode; return; }
    Node* temp = head;
    while (temp->next)
        temp = temp->next;
    temp->next = newNode;
}

// Helper function to compare timestamps (for std::sort)
bool compareByTimestamp(const UserAction& a, const UserAction& b) {
    return a.timestamp < b.timestamp;
}

// Utility to clear the linked list nodes
void clearLinkedList(Node*& head) {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr; // Sets the caller's pointer to nullptr after deallocation
}

// Save all actions (formatted table + sorted)
bool FileIO::saveActionsToFile(Node*& head, const string& filename) {
    if (!head) {
        cerr << "Error: No actions to save!\n";
        return false;
    }

    // 1. Count nodes to determine array size
    int count = 0;
    Node* temp = head;
    while (temp) {
        count++;
        temp = temp->next;
    }

    if (count == 0) {
        return false;
    }

    // 2. Dynamically allocate a C-style array for sorting
    UserAction* actions = new UserAction[count];

    // 3. Populate the array (linked list to array conversion)
    temp = head;
    for (int i = 0; i < count; ++i) {
        actions[i] = temp->data;
        temp = temp->next;
    }

    // 4. Sort the array using std::sort (from <algorithm>)
    std::sort(actions, actions + count, compareByTimestamp);

    // 5. Open file and write data
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "' for writing.\n";
        delete[] actions; // Clean up array before exiting
        return false;
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

    // Write actions directly from the sorted array (FIFO order)
    for (int i = 0; i < count; ++i) {
        const auto& act = actions[i];
        // Convert time_t to readable string
        char timeStr[25];
        if (strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&act.timestamp)) == 0) {
            strcpy(timeStr, "TIME_ERROR");
        }

        file << left << setw(25) << timeStr
             << setw(10) << act.userID
             << setw(20) << act.action
             << setw(20) << act.processName
             << setw(12) << fixed << setprecision(2) << act.duration
             << setw(18) << act.nextAction
             << setw(10) << act.status << endl;
    }

    file << string(120, '=') << "\n\n";
    file.close();

    // 6. Deallocate array and linked list
    delete[] actions;
    clearLinkedList(head);

    cout << "\n✅ Actions saved to file '" << filename << "' in table format and sorted (FIFO order).\n";
    return true;
}

// Read file and print to console (formatted)
void FileIO::readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "' for reading.\n";
        return;
    }

    string line;
    cout << "\nFile Contents:\n";
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

// Save a single action entry (unformatted, appended)
bool FileIO::saveAction(const UserAction& action, const string& filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "' for appending.\n";
        return false;
    }

    file << "UserID: " << action.userID
         << ", Action: " << action.action
         << ", Process: " << action.processName
         << ", Duration: " << action.duration
         << ", Timestamp: " << action.timestamp
         << ", NextAction: " << action.nextAction
         << ", Status: " << action.status << "\n";

    file.close();
    return true;
}
