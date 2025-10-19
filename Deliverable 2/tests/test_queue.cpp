#include "../include/queue_monitor.h"
#include "../include/user_action.h"
#include <iostream>
#include <ctime>

using namespace std;

int main() {
    QueueMonitor q;
    time_t now = time(nullptr);

    UserAction a1("U101", "Login", "System", 5.0, now - 180, "OpenFile", "Success"); // Front
    UserAction a2("U102", "Logout", "System", 3.0, now, "-", "Success");
    UserAction a3("U103", "Process_Start", "TaskMgr", 1.5, now + 60, "Monitor", "Running"); // Rear


    cout << "\n--- Enqueueing Actions ---\n";
    q.enqueueAction(a1);
    q.enqueueAction(a2);
    q.enqueueAction(a3);

    cout << "\n--- Queue Status ---\n";
    cout << "Queue Size: " << q.getSize() << endl;
    q.peekFront();

    // --- Test Copy ---
    cout << "\n--- Testing Copy To ---\n";
    QueueMonitor qCopy;
    q.copyTo(qCopy);
    cout << "Copied Queue Size: " << qCopy.getSize() << endl;
    cout << "Copied Queue Front (Should be U101):\n";
    qCopy.peekFront();

    // --- Test Clear ---
    cout << "\n--- Testing Clear ---\n";
    q.clearQueue();
    cout << "Original Queue Size after clear: " << q.getSize() << endl;


    cout << "\n--- Dequeueing Copied Actions (FIFO) ---\n";
    qCopy.dequeueAction(); // Should be U101
    qCopy.peekFront();     // Should be U102
    qCopy.dequeueAction(); // Should be U102
    qCopy.dequeueAction(); // Should be U103

    cout << "\n--- Empty Queue Test ---\n";
    qCopy.dequeueAction(); // test empty queue

    return 0;
}
