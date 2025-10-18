#include "../include/linked_list.h"
#include "../include/user_action.h"
#include <iostream>
using namespace std;

int main() {
    LinkedList list;

    list.append(UserAction("2025-10-18 10:10", "U301", "Start_App", "browser.exe", "10s", "Load_Page", "Success"));
    list.append(UserAction("2025-10-18 10:20", "U302", "Stop_App", "browser.exe", "5s", "-", "Closed"));

    cout << "\nDisplaying Linked List:\n";
    list.display();

    return 0;
}
