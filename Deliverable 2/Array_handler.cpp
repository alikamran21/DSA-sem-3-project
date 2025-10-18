#include <iostream>
#include <ctime>
#include "Array_handler.h"
#include "Utils.h"

using namespace std;


arrayHandler::arrayHandler()
{
    tasks = new Task[maxTask];
    taskCounter = 0;
}
arrayHandler ::~arrayHandler() // destroying hehe
{
    delete[] tasks;
}

void arrayHandler::addTask(const string &description) // To add the tasks in an array 
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
        cout << "Session Empty. No tasks found.\n";
        return;
    }

    int idNo;
    cout << "Enter the Task ID you want to remove: ";

    if (!(cin >> idNo)) { 
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input! Please enter a number.\n";
        return;
    }

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



void arrayHandler::showTaskList() // Displaying the lists of tasks done till now
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
int arrayHandler::getTaskCounter() // counting the tasks being done
{
    return taskCounter;
}

void arrayHandler::clearTask() // clear all the task history 
{
    taskCounter = 0;
    cout << "Cleared History. ;)";
}
