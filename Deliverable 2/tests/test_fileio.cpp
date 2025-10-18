 #include "../include/file_io.h"
#include "../include/user_action.h"
#include <iostream>
using namespace std;

int main() {
    Node* head = nullptr;

    // Add sample actions
    appendNode(head, UserAction("2025-10-18 21:22:04", "U101", "Open_File", "editor.exe", "120ms", "Edit_Doc", "Normal"));
    appendNode(head, UserAction("2025-10-18 21:30:14", "U102", "Save_File", "editor.exe", "80ms", "Close_File", "Normal"));
    appendNode(head, UserAction("2025-10-18 21:40:50", "U103", "Close_File", "editor.exe", "60ms", "-", "Normal"));

    string filename = "data/user_logs.txt";
    FileIO::saveActionsToFile(head, filename);

    cout << "\n--- Reading Back from File ---\n";
    FileIO::readFile(filename);

    return 0;
}
