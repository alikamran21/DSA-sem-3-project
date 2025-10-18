#include "../include/stack_monitor.h"
#include "../include/user_action.h"
#include <iostream>
#include <ctime>

using namespace std;

int main() {
    StackMonitor stack;
    time_t now = time(nullptr);

    UserAction a1("U201", "Open", "Notepad", 2.0, now - 300, "Save", "Running");
    UserAction a2("U202", "Close", "Notepad", 1.0, now, "-", "Done");
    UserAction a3("U203", "Error", "Driver", 0.5, now + 60, "Restart", "Failure");


    cout << "\n--- Pushing Actions ---\n";
    stack.pushAction(a1);
    stack.pushAction(a2);
    stack.pushAction(a3);

    cout << "\n--- Stack Status ---\n";
    // FIX: Method name changed from peekTop() to peekAction()
    stack.peekAction();

    cout << "\n--- Display All Actions ---\n";
    stack.displayAll();

    cout << "\n--- Popping Actions ---\n";
    stack.popAction();
    stack.popAction();
    stack.popAction();

    cout << "\n--- Empty Stack Test ---\n";
    stack.popAction(); // Empty pop test

    return 0;
}
