#ifndef FILE_IO_H
#define FILE_IO_H
#include <iostream>
#include <fstream>
#include <string>
#include "user_action.h" // Requires UserAction constructor parameter for time_t to be renamed (e.g., from 'time' to 'ts')
#include "linked_list.h"
using namespace std;

// Node structure for the basic singly linked list used internally by FileIO.
struct Node {
    UserAction data;
    Node* next;
    Node(const UserAction& action);
};

// Handles saving and reading user actions from text files
class FileIO {
public:
    // Saves a single action entry (unformatted, appended)
    static bool saveAction(const UserAction& action, const string& filename);
    // Reads and displays file contents to the console
    static void readFile(const string& filename);
    // Saves a linked list of actions as a formatted, sorted table.
    // NOTE: Node*& head allows for memory cleanup within the function.
    static bool saveActionsToFile(Node*& head, const string& filename);
};

// Linked list utility: adds a new action node to the end of the list
void appendNode(Node*& head, const UserAction& action);
static bool loadActionsFromFile(const string& filename, LinkedList& list);

#endif // FILE_IO_H
