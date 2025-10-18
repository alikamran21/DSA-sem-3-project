#ifndef FILE_IO_H
#define FILE_IO_H

#include <iostream>
#include <fstream>
#include <string>
#include "user_action.h"

using namespace std;

struct Node {
    UserAction data;
    Node* next;
    Node(const UserAction& action);
};

// Handles saving and reading user actions from text files
class FileIO {
public:
    static bool saveAction(const UserAction& action, const string& filename);
    static void readFile(const string& filename);
    static bool saveActionsToFile(Node* head, const string& filename);
};

// Linked list utility
void appendNode(Node*& head, const UserAction& action);

#endif
