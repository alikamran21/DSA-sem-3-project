#ifndef Array_handler_h
#define Array_handler_h

#include <iostream>
#include <cstring>
using namespace std;

const int maxTask = 999 ;

/*
 * arrayHandler
 * ------------
 * Manages a fixed-size array of Task structures.
 * Provides:
 * - addTask(): adds a new task description
 * - removeTask(): removes the last added task
 * - showTaskList(): displays all current tasks
 * - clearTask(): resets the task counter
 *
 * Struct stores:
 * Task { id, description, timeStamp }
 */

struct Task
{
    int id;
    string description;
    string timeStamp;
};

class arrayHandler
{
private:
    Task* tasks;
    int taskCounter;
public:
    arrayHandler();
    ~arrayHandler();

    void addTask(const string& description);
    void removeTask();
    void showTaskList();
    int getTaskCounter();

    void clearTask();
};
 #endif