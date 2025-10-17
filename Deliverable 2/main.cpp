#include "array_handler.h"
#include "circular_queue.h"
#include "pointer_utils.h"
#include "utils.h"
#include <iostream>
using namespace std;
using namespace PointerUtils;

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


    cout << "=========================\n";
    cout << " POINTER UTILS TESTING\n";
    cout << "=========================\n\n";

    int* numPtr = (int*) allocate(sizeof(int), "int");
    if (numPtr) {
        *numPtr = 42;
        showPointerDetails(numPtr, "numPtr");
    }

    double* arrPtr = (double*) allocate(5 * sizeof(double), "double[5]");
    for (int i = 0; i < 5; ++i) arrPtr[i] = i * 1.1;

    showMemoryUsage();

    deallocate(numPtr, "int");

    showPointerDetails(numPtr, "numPtr");

    deallocate(arrPtr, "double[5]");
    showMemoryUsage();

    return 0;
}
