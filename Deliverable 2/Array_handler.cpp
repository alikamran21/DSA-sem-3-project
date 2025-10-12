#include <iostream>
#include <ctime>
#include "Array_handler.h"

using namespace std;

string currentTime()
{
    time_t now = time(0);
    char tNow[80];
    strftime(tNow, sizeof(tNow), "%H:%M:%S", localtime(&now));
    return string(tNow);
}

arrayHandler::arrayHandler()
{
    tasks = new Task[maxTask];
    taskCounter = 0;
}
arrayHandler ::~arrayHandler()
{
    delete[] tasks;
}

void arrayHandler::addTask(const string &description)
{
    if (taskCounter >= maxTask)
    {
        cout << "Max tasks Reached cannot store more :( ";
        return;
    }
    tasks[taskCounter].id = taskCounter + 1;
    tasks[taskCounter].description = description;
    tasks[taskCounter].timeStamp = currentTime();
    taskCounter++;
    cout << "Task Added :)" << description << endl;
}

void arrayHandler::removeTask() {
    if (taskCounter == 0) {
        cout << "Session Empty NO tasks found.\n";
        return;
    }

    int idNo;
    cout << "Enter the Task ID you want to remove: ";
    cin >> idNo;

    if (idNo <= 0 || idNo > taskCounter) {
        cout << "Invalid Task Id provided. :(\n";
        return;
    }

    cout << "Removing Task: [" << tasks[idNo - 1].id << "] "
         << tasks[idNo - 1].description << endl;

    for (int i = idNo - 1; i < taskCounter - 1; ++i) {
        tasks[i] = tasks[i + 1];
        tasks[i].id = i + 1; 
    }

    taskCounter--;
    cout << "Task removed. :)\n";
}


void arrayHandler::showTaskList()
{
    if (taskCounter == 0)
    {
        cout << "Session Empty.";
        return;
    }
    cout << "Session Task List:\n";
    for (int i = 0; i < taskCounter; i++)
    {
        cout << "[" << tasks[i].id << "] " 
     << tasks[i].description 
     << " (" << tasks[i].timeStamp << ")\n";

    }
    cout << endl;
}
int arrayHandler::getTaskCounter()
{
    return taskCounter;
}

void arrayHandler::clearTask()
{
    taskCounter = 0;
    cout << "Cleared History. ;)";
}
