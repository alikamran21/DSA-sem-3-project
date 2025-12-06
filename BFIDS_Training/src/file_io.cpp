#include "file_io.h"
#include "linked_list.h"
#include <iomanip>
#include <sstream>
#include <algorithm> // For std::sort
#include <ctime>
#include <limits>
#include <cstring>   // For strcpy

using namespace std;

/*
    file_io.cpp
    -----------
    Handles all file input/output operations.
    Includes utilities for saving logs, loading histories, and formatting output.
    Also handles conversion between Linked Lists and Arrays for sorting.
*/

// Node constructor defined here as declared in file_io.h
Node::Node(const UserAction& action) : data(action), next(nullptr) {}

/*
    appendNode
    ----------
    Helper function to append a new node to a raw singly linked list.
*/
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

/*
    clearLinkedList
    ---------------
    Recursively deletes a raw singly linked list to prevent memory leaks.
*/
void clearLinkedList(Node*& head) {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr; // Sets the caller's pointer to nullptr after deallocation
}

/*
    saveActionsToFile
    -----------------
    Saves a linked list of actions to a file in a formatted table.
    1. Converts List -> Array.
    2. Sorts the Array by timestamp.
    3. Writes formatted output.
*/
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

    cout << "\nActions saved to file '" << filename << "' in table format and sorted (FIFO order).\n";
    return true;
}

/*
    readFile
    --------
    Reads a file and dumps its content to the console.
    Useful for debugging or verification.
*/
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

/*
    saveAction
    ----------
    Appends a single user action to a log file.
    Does NOT overwrite existing data (Append Mode).
*/
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

/*
    loadActionsFromFile
    -------------------
    Reads actions from a log file and populates a LinkedList.
    Parses each line to extract UserAction fields.
*/
bool FileIO::loadActionsFromFile(const string& filename, LinkedList& list) {
    ifstream file(filename);
    if (!file.is_open()) {
        // Fallback: create some data if file not found (helps with testing)
        cerr << "Warning: Could not open input log file. Using fallback data.\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        UserAction ua;
        string ignore;

        // Assumes format: UserID: U101, Action: Login, ...
        stringstream ss(line);
        
        // Simple token extraction (works best for space separated values)
        ss >> ua.userID >> ua.action >> ua.processName >> ua.duration >> ua.timestamp >> ua.nextAction >> ua.status;
        
        // If that fails, try the label skipping method:
        if (ss.fail()) {
             ss.clear();
             ss.str(line);
             ss >> ignore >> ua.userID;      // skip "UserID:"
             ss >> ignore >> ua.action;      // skip "Action:"
             ss >> ignore >> ua.processName; // skip "Process:"
             ss >> ignore >> ua.duration;    // ...
             ss >> ignore >> ua.timestamp;
             ss >> ignore >> ua.nextAction;
             ss >> ignore >> ua.status;
        }

        list.insertAtEnd(ua);
    }

    return true;
}