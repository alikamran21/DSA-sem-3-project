 #include "../include/queue_monitor.h"
#include "../include/user_action.h"
#include <iostream>
using namespace std;

int main() {
    QueueMonitor q;
    UserAction a1("2025-10-18 12:00", "U101", "Login", "System", "5s", "OpenFile", "Success");
    UserAction a2("2025-10-18 12:10", "U102", "Logout", "System", "3s", "-", "Success");

    q.enqueueAction(a1);
    q.enqueueAction(a2);

    q.peekFront();
    q.dequeueAction();
    q.dequeueAction();
    q.dequeueAction();  // test empty queue

    return 0;
}
