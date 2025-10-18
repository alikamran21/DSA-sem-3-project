#include "../include/queue_monitor.h"
#include "../include/user_action.h"
#include <iostream>
#include <ctime>

using namespace std;

int main() {
    QueueMonitor q;
    time_t now = time(nullptr);

    // UserAction(userID, action, processName, duration (double), timestamp (time_t), nextAction, status)
    UserAction a1("U101", "Login", "System", 5.0, now - 180, "OpenFile", "Success");
    UserAction a2("U102", "Logout", "System", 3.0, now, "-", "Success");
    UserAction a3("U103", "Process_Start", "TaskMgr", 1.5, now + 60, "Monitor", "Running");


    cout << "\n--- Enqueueing Actions ---\n";
    q.enqueueAction(a1);
    q.enqueueAction(a2);
    q.enqueueAction(a3);

    cout << "\n--- Queue Status ---\n";
    cout << "Queue Size: " << q.getSize() << endl;
    q.peekFront();

    cout << "\n--- Dequeueing Actions ---\n";
    q.dequeueAction();
    q.peekFront();
    q.dequeueAction();
    q.dequeueAction();

    cout << "\n--- Empty Queue Test ---\n";
    q.dequeueAction();  // test empty queue

    return 0;
}
