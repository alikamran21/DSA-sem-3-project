#include <iostream>
#include <ctime>
#include <string>
#include <vector>

// Include all necessary headers from the project's 'include' directory
#include "../include/user_action.h"
#include "Circular_queue.h"
#include "Array_handler.h"
#include "../include/linked_list.h"
#include "../include/stack_monitor.h"
#include "../include/queue_monitor.h"
#include "../include/File_io.h"
#include "Utils.h"
#include "Pointer_utils.h"

using namespace std;

// Node structure and utility function required for FileIO::saveActionsToFile demo
struct Node {
    UserAction data;
    Node* next;
    Node(const UserAction& action) : data(action), next(nullptr) {}
};

void appendNode(Node*& head, const UserAction& action) {
    Node* newNode = new Node(action);
    if (!head) { head = newNode; return; }
    Node* temp = head;
    while (temp->next)
        temp = temp->next;
    temp->next = newNode;
}

void processAction(
    const UserAction& action,
    Array_handler& arrayHandler,
    Circular_queue& circularQueue,
    LinkedList& sessionHistory,
    StackMonitor& actionStack
) {
    cout << "\n======================================================\n";
    cout << "PROCESSING ACTION: " << action.action
         << " by " << action.userID
         << " (" << action.duration << "s)\n";
    cout << "Timestamp: " << Utils::formatTimestamp(action.timestamp) << endl;
    cout << "======================================================\n";

    // Log to session history and stack
    sessionHistory.insertAtEnd(action);
    actionStack.pushAction(action);

    // Anomaly Detection: Duration Check
    bool durationAnomaly = arrayHandler.checkDurationAnomaly(action);
    if (durationAnomaly) {
        cout << "ANOMALY ALERT: Duration deviated significantly from baseline!\n";
    }

    // Anomaly Detection: Sequence Check
    circularQueue.enqueueAction(action);
    bool sequenceAnomaly = circularQueue.checkFailedLoginSpike();
    if (sequenceAnomaly) {
        cout << "CRITICAL ALERT: Suspicious activity spike detected in sliding window!\n";
    }

    // Update Baseline (if action was not anomalous)
    if (!durationAnomaly) {
        arrayHandler.addBaseline(action);
    }

    circularQueue.display();
}

int main() {
    cout << "======================================================\n";
    cout << "   BEHAVIORAL FINGERPRINT INTRUSION DETECTION SYSTEM\n";
    cout << "======================================================\n";

    // 1. Initialize Data Structure Modules
    Array_handler baselineHandler;
    Circular_queue slidingWindow;
    LinkedList sessionHistory;
    StackMonitor actionStack;
    QueueMonitor taskQueue;

    time_t now = time(nullptr);

    // 2. Build Baseline Data (Normal Behavior)
    cout << "\n--- 2. BUILDING INITIAL BASELINE (Array_handler) ---\n";

    baselineHandler.addBaseline(UserAction("U101", "Login", "OS", 3.0, now, "RunApp", "Success"));
    baselineHandler.addBaseline(UserAction("U101", "RunApp", "appX.exe", 10.0, now, "SaveFile", "Normal"));
    baselineHandler.addBaseline(UserAction("U101", "SaveFile", "appX.exe", 5.0, now, "Logout", "Normal"));
    baselineHandler.addBaseline(UserAction("U101", "RunApp", "appX.exe", 11.0, now, "Logout", "Normal"));
    baselineHandler.addBaseline(UserAction("U101", "SaveFile", "appX.exe", 6.0, now, "Logout", "Normal"));

    // 3. Simulate User Session with Anomaly Detection
    cout << "\n\n--- 3. SIMULATING USER SESSION AND ANOMALY DETECTION ---\n";

    // Scenario 1: Normal Operation
    processAction(UserAction("U101", "CheckStatus", "monitor.exe", 2.0, now + 10, "RunApp", "Normal"),
                  baselineHandler, slidingWindow, sessionHistory, actionStack);

    // Scenario 2: Sequence Anomaly Setup (Failed Logins)
    processAction(UserAction("U101", "Login_Attempt", "OS", 1.0, now + 20, "Login", "Unauthorized_Login"),
                  baselineHandler, slidingWindow, sessionHistory, actionStack);

    // Scenario 3: Sequence Anomaly Setup
    processAction(UserAction("U101", "Login_Attempt", "OS", 1.0, now + 25, "Login", "Unauthorized_Login"),
                  baselineHandler, slidingWindow, sessionHistory, actionStack);

    // Scenario 4: Sequence Anomaly Trigger (CRITICAL ALERT expected)
    processAction(UserAction("U101", "Network_Connect", "cmd.exe", 1.2, now + 30, "Transfer", "Attempting"),
                  baselineHandler, slidingWindow, sessionHistory, actionStack);

    // Scenario 5: Duration Anomaly Check (ANOMALY ALERT expected)
    processAction(UserAction("U101", "RunApp", "appX.exe", 1.0, now + 40, "SaveFile", "Normal"),
                  baselineHandler, slidingWindow, sessionHistory, actionStack);

    // 4. Demonstrate Other Data Structures (LIFO/FIFO)
    cout << "\n\n--- 4. DEMONSTRATING STACK (LIFO) AND QUEUE (FIFO) ---\n";

    // Queue Demonstration (Pending Tasks)
    cout << "\n--- Queue Monitor (FIFO) ---\n";
    taskQueue.enqueueAction(UserAction("S001", "SysTask_A", "Kernel", 0.0, now + 50, "", "Pending"));
    taskQueue.enqueueAction(UserAction("S001", "SysTask_B", "Kernel", 0.0, now + 55, "", "Pending"));
    taskQueue.peekFront();
    taskQueue.dequeueAction();
    taskQueue.peekFront();

    // Stack Demonstration (Nested/Reversible Operations)
    cout << "\n--- Stack Monitor (LIFO) ---\n";
    actionStack.peekAction();
    actionStack.displayAll();
    actionStack.popAction();
    actionStack.popAction();
    actionStack.peekAction();

    // 5. Demonstrate Persistence (File I/O)
    cout << "\n\n--- 5. DEMONSTRATING PERSISTENCE (FileIO) ---\n";

    // Create a new linked list for the FileIO function's input
    Node* fileIOHead = nullptr;
    appendNode(fileIOHead, UserAction("U101", "CheckStatus", "monitor.exe", 2.0, now + 10, "RunApp", "Normal"));
    appendNode(fileIOHead, UserAction("U101", "Login_Attempt", "OS", 1.0, now + 20, "Login", "Unauthorized_Login"));
    appendNode(fileIOHead, UserAction("U101", "Login_Attempt", "OS", 1.0, now + 25, "Login", "Unauthorized_Login"));
    appendNode(fileIOHead, UserAction("U101", "Network_Connect", "cmd.exe", 1.2, now + 30, "Transfer", "Attempting"));
    appendNode(fileIOHead, UserAction("U101", "RunApp", "appX.exe", 1.0, now + 40, "SaveFile", "Normal"));

    string log_filename = "data/bfids_session_log.txt";

    FileIO::saveActionsToFile(fileIOHead, log_filename);

    FileIO::readFile(log_filename);

    Pointer_utils::trackDeallocation("All Data Structures");

    cout << "\n======================================================\n";
    cout << "     SYSTEM SHUTDOWN: Data Structures Cleaned Up\n";
    cout << "======================================================\n";

    return 0;
}
