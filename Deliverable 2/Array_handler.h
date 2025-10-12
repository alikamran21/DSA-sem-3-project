#ifndef Array_handler_h
#define Array_handler_h

#include <iostream>
#include <cstring>
using namespace std;

const int maxTask = 999 ;

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