#include "../include/file_io.h"
#include "../include/user_action.h"
#include <iostream>
#include <ctime>

using namespace std;

int main() {
    // Start with an empty head pointer
    Node* head = nullptr;

    // Use current time to generate realistic timestamps
    time_t now = time(nullptr);

    // UserAction(userID, action, processName, duration (double), timestamp (time_t), nextAction, status)
    // NOTE: Timestamps are out of order to ensure the sort function works.
    appendNode(head, UserAction("U101", "Open_File", "editor.exe", 120.5, now - 600, "Edit_Doc", "Normal")); // Oldest
    appendNode(head, UserAction("U104", "Network_Call", "cmd.exe", 1.2, now + 10, "Exit", "Deviated"));      // Newest
    appendNode(head, UserAction("U102", "Save_File", "editor.exe", 80.25, now - 300, "Close_File", "Normal"));
    appendNode(head, UserAction("U103", "Close_File", "editor.exe", 60.0, now, "-", "Normal"));              // Middle

    cout << "Initial list populated with 4 actions.\n";

    string filename = "data/user_logs.txt";

    // Demonstrate saving the list of actions
    // FileIO::saveActionsToFile is called with Node*& head. The function will
    // clean up the linked list memory and set 'head' to nullptr.
    FileIO::saveActionsToFile(head, filename);

    // Verify that the list was cleaned up.
    if (head == nullptr) {
        cout << "\nMemory check: 'head' is correctly set to nullptr after saving and cleanup. (SUCCESS)\n";
    } else {
        cout << "\nMemory check: 'head' is NOT nullptr after saving. Possible memory leak. (ERROR)\n";
    }

    cout << "\n--- Reading Back from File (Should be sorted by time: U101, U102, U103, U104) ---\n";
    FileIO::readFile(filename);

    return 0;
}
