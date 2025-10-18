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
    stack.pushAction(a1); // Bottom
    stack.pushAction(a2);
    stack.pushAction(a3); // Top

    cout << "\n--- Stack Status ---\n";
    stack.peekAction();
    stack.displayAll();

    // --- Test Reverse ---
    cout << "\n--- Reversing Stack (U203 should be bottom) ---\n";
    stack.reverseStack();
    stack.displayAll();

    // --- Test Copy ---
    cout << "\n--- Copying Stack ---\n";
    StackMonitor stackCopy;
    stack.copyTo(stackCopy);
    cout << "Copied Stack Contents:\n";
    stackCopy.displayAll();

    // --- Test Clear ---
    cout << "\n--- Clearing Original Stack ---\n";
    stack.clearStack();
    cout << "Original Stack Size after clear: " << stack.getSize() << endl;

    cout << "\n--- Popping Actions from Copied Stack (Reverse order) ---\n";
    stackCopy.popAction(); // Should be U201
    stackCopy.popAction(); // Should be U202
    stackCopy.popAction(); // Should be U203

    cout << "\n--- Empty Stack Test ---\n";
    stackCopy.popAction(); // Empty pop test

    return 0;
}
