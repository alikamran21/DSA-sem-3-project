#include "../include/linked_list.h"
#include "../include/user_action.h"
#include <iostream>
#include <ctime>

using namespace std;

int main() {
    LinkedList list;
    time_t now = time(nullptr);

    // UserAction(userID, action, processName, duration (double), timestamp (time_t), nextAction, status)
    UserAction a1("U301", "Start_App", "browser.exe", 10.0, now - 120, "Load_Page", "Success");
    UserAction a2("U302", "Stop_App", "browser.exe", 5.0, now, "-", "Closed");
    UserAction a3("U303", "Download_File", "browser.exe", 45.3, now + 60, "Exit_Process", "Running");

    // FIX: Use correct method names from linked_list.h
    list.insertAtEnd(a1);       // Replaces list.append()
    list.insertAtBeginning(a2);
    list.insertAtEnd(a3);

    cout << "\nDisplaying Linked List Forward:\n";
    list.displayForward();      // Replaces list.display()

    cout << "\nDisplaying Linked List Backward:\n";
    list.displayBackward();

    cout << "\nList Size: " << list.getSize() << endl;

    // Test deletion
    list.deleteFromBeginning();
    list.deleteFromEnd();
    list.displayForward();
    cout << "\nList Size after deletion: " << list.getSize() << endl;

    return 0;
}
