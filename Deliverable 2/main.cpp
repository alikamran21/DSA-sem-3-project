#include <iostream>
#include <ctime>
#include <string>
#include <iomanip>
#include <algorithm>

// --- A's Headers ---
#include "Test-A/Array_handler.h"
#include "Test-A/circular_queue.h"
#include "Test-A/pointer_utils.h"
#include "Test-A/utils.h"

// --- F's Headers ---
#include "Test-F/include/user_action.h"
#include "Test-F/include/stack_monitor.h"
#include "Test-F/include/queue_monitor.h"
#include "Test-F/include/linked_list.h"
#include "Test-F/include/file_io.h"

using namespace std;
using namespace PointerUtils;

// --- Main execution block ---

int main() {
    time_t now = time(nullptr);
    cout << "========================================================\n";
    cout << "          DSA PROJECT - COMBINED MODULE TEST          \n";
    cout << "========================================================\n\n";

    // =============================================================
    // (Using UserAction struct, StackMonitor, QueueMonitor, LinkedList, FileIO)
    // =============================================================
    cout << "--- 1. MODULES (Stack, Queue, DLL, FileIO) ---\n";

    // --- A. StackMonitor Test (LIFO Linked List Stack) ---
    cout << "\n--- A. Stack Monitor Test ---\n";
    StackMonitor stack;
    UserAction fa1("U201", "Open", "Notepad", 2.0, now - 300, "Save", "Running");
    UserAction fa2("U202", "Close", "Notepad", 1.0, now, "-", "Done");
    UserAction fa3("U203", "Error", "Driver", 0.5, now + 60, "Restart", "Failure");

    stack.pushAction(fa1);
    stack.pushAction(fa2);
    stack.pushAction(fa3);

    stack.peekAction();
    stack.displayAll();

    StackMonitor stackCopy;
    stack.copyTo(stackCopy);
    cout << "Popping from copied stack:\n";
    stackCopy.popAction(); // Should be U201 (due to copy implementation logic)
    stack.clearStack();

    // --- B. QueueMonitor Test (FIFO Linked List Queue) ---
    cout << "\n--- B. Queue Monitor Test ---\n";
    QueueMonitor q;
    UserAction fq1("U101", "Login", "System", 5.0, now - 180, "OpenFile", "Success");
    UserAction fq2("U102", "Logout", "System", 3.0, now, "-", "Success");
    
    q.enqueueAction(fq1); // Front
    q.enqueueAction(fq2); 

    q.peekFront();
    q.dequeueAction(); // Removes U101
    q.peekFront();     // Shows U102
    q.clearQueue();

    // --- C. LinkedList Test (Doubly Linked List) ---
    cout << "\n--- C. Doubly Linked List Test ---\n";
    LinkedList list;
    UserAction fl1("U301", "Start_App", "browser.exe", 10.0, now - 120, "Load_Page", "Success");
    UserAction fl2("U302", "Stop_App", "browser.exe", 5.0, now, "-", "Closed");

    list.insertAtEnd(fl1);
    list.insertAtBeginning(fl2);

    list.displayForward();
    list.displayBackward();

    list.deleteFromBeginning(); // Deletes U302
    list.displayForward();


    // --- D. FileIO Test (Sorting and Saving) ---
    cout << "\n--- D. File IO Test (Saving Sorted Actions) ---\n";
    Node* head = nullptr; 
    string filename = "user_logs.txt";

    // Appending actions out of order to ensure sorting works
    appendNode(head, UserAction("U101", "Open_File", "editor.exe", 120.5, now - 600, "Edit_Doc", "Normal")); 
    appendNode(head, UserAction("U104", "Network_Call", "cmd.exe", 1.2, now + 10, "Exit", "Deviated"));      
    appendNode(head, UserAction("U102", "Save_File", "editor.exe", 80.25, now - 300, "Close_File", "Normal"));
    
    // Save, sort by timestamp (FIFO order), and clean up memory
    FileIO::saveActionsToFile(head, filename);

    // Read back contents to confirm sorting
    FileIO::readFile(filename);
    
    // =============================================================
    // (Using Task/Action structs, ArrayHandler, circularQueue, PointerUtils)
    // =============================================================
    cout << "\n========================================================\n";
    cout << "--- 2. MODULES (Array, Circular Queue, Memory Mgr) ---\n";

    // --- E. ArrayHandler Test (Dynamic Array) ---
    cout << "\n--- E. Array Handler Test ---\n";
    arrayHandler handler;

    handler.addTask("Finish DSA project");
    handler.addTask("Review header files");
    handler.showTaskList();
    
    // Skipping interactive part (removeTask) for simplified test flow
    handler.clearTask();
    handler.showTaskList();

    // --- F. circularQueue Test (Circular Buffer/Sliding Window) ---
    cout << "\n--- F. Circular Queue Test (Capacity 5) ---\n";
    circularQueue recentActions(5); 

    for (int i = 1; i <= 7; ++i) { // Test overflow/overwrite (2 oldest items lost)
        Action a;
        a.user = "UserA";
        a.description = "Pressed key #" + to_string(i);
        a.timeStamp = currentTime();
        recentActions.enqueue(a);
    }
    // Queue should contain entries #3, #4, #5, #6, #7
    recentActions.displayAll(); 

    Action oldest = recentActions.peek();
    cout << "Oldest record still in queue: " << oldest.description << endl; 

    recentActions.dequeue(); // Removes #3
    recentActions.displayAll();

    recentActions.clear();
    
    // --- G. PointerUtils Test (Custom Memory Manager) ---
    cout << "\n--- G. Pointer Utilities Test ---\n";

    int* numPtr = (int*) allocate(sizeof(int), "int");
    if (numPtr) {
        *numPtr = 42;
        showPointerDetails(numPtr, "numPtr");
    }

    double* arrPtr = (double*) allocate(5 * sizeof(double), "double[5]");

    showMemoryUsage();

    deallocate(numPtr, "int");

    showPointerDetails(numPtr, "numPtr"); // Should show as invalid/freed

    deallocate(arrPtr, "double[5]");
    showMemoryUsage();

    cout << "\n========================================================\n";

    return 0;
}