#include "array_handler.h"
#include "circular_queue.h"
#include "utils.h"
#include <iostream>
using namespace std;


int main() {
    cout << "=========================\n";
    cout << "  ARRAY HANDLER TESTING\n";
    cout << "=========================\n\n";

    arrayHandler handler;

    handler.addTask("Finish DSA project");
    handler.addTask("Review header files");
    handler.addTask("Test array handler module");

    handler.showTaskList();

    handler.removeTask();
    handler.showTaskList();

    handler.clearTask();
    handler.showTaskList();

    cout << "\n=============================\n";
    cout << "   CIRCULAR QUEUE TESTING\n";
    cout << "=============================\n\n";

    circularQueue recentActions(5); 

    for (int i = 1; i <= 7; ++i) {
        Action a;
        a.user = "UserA";
        a.description = "Pressed key #" + to_string(i);
        a.timeStamp = currentTime();
        recentActions.enqueue(a);
    }

    recentActions.displayAll();

    Action loginB = {"UserB", "Logged in", currentTime()};
    recentActions.enqueue(loginB);
    Action clickB = {"UserB", "Clicked dashboard button", currentTime()};
    recentActions.enqueue(clickB);

    recentActions.displayAll();

    Action oldest = recentActions.peek();
    cout << "Oldest record still in queue: " << oldest.description << endl;

    recentActions.dequeue();
    recentActions.displayAll();

    cout << "\nCombined test complete.\n";
    return 0;
}
