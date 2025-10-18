#include "../include/stack_monitor.h"
#include "../include/user_action.h"
#include <iostream>
using namespace std;

int main() {
    StackMonitor stack;
    UserAction a1("2025-10-18 14:00", "U201", "Open", "Notepad", "2s", "Save", "Running");
    UserAction a2("2025-10-18 14:05", "U202", "Close", "Notepad", "1s", "-", "Done");

    stack.pushAction(a1);
    stack.pushAction(a2);

    stack.peekTop();
    stack.popAction();
    stack.popAction();
    stack.popAction(); // Empty pop test

    return 0;
}
