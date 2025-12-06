#include <iostream>
#include <ctime>
#include "Array_handler.h"
#include "utils.h"

using namespace std;

/*
    Array_handler.cpp
    -----------------
    Manages a simple fixed-size array of Task structures.
    This simulates a basic session manager where tasks (actions) are logged sequentially.
    
    Features:
        - Add/Remove tasks
        - Display history
        - Clear session
*/

// Constructor
// Runs automatically when a new arrayHandler object is created.
// Allocates memory for the task list.
arrayHandler::arrayHandler()
{
    tasks = new Task[maxTask]; // dynamically allocating array of Task objects
    taskCounter = 0;           // keeping track of how many tasks are added
}

// Destructor
// Runs automatically when object is destroyed.
// Frees the dynamically allocated memory.
arrayHandler::~arrayHandler()
{
    delete[] tasks;
}

/*
    addTask
    -------
    Adds a new task to the array.
    Checks for overflow before adding.
    Assigns a unique ID and current timestamp to the task.
*/
void arrayHandler::addTask(const string &description)
{
    // Stop if we’ve already reached our max limit
    if (taskCounter >= maxTask)
    {
        cout << "Max tasks reached! Cannot store more :( " << endl;
        return;
    }

    // Assign task details
    tasks[taskCounter].id = taskCounter + 1;      // simple sequential ID
    tasks[taskCounter].description = description; // store what the user typed
    tasks[taskCounter].timeStamp = currentTime(); // record when it was added

    taskCounter++; // increase the count of tasks
    cout << "Task Added :) " << description << endl;
}

/*
    removeTask
    ----------
    Removes a task by its ID.
    Shifts subsequent tasks to fill the gap (Array Shift Operation).
*/
void arrayHandler::removeTask()
{
    // If there’s nothing to remove
    if (taskCounter == 0)
    {
        cout << "Session Empty. No tasks found.\n";
        return;
    }

    int idNo;
    cout << "Enter the Task ID you want to remove: ";

    // Check if the input is valid (only numbers allowed)
    if (!(cin >> idNo))
    {
        cin.clear();             // clear input error flag
        cin.ignore(10000, '\n'); // discard invalid input
        cout << "Invalid input! Please enter a number.\n";
        return;
    }

    // Validate task ID range
    if (idNo <= 0 || idNo > taskCounter)
    {
        cout << "Invalid Task Id provided. :(\n";
        return;
    }

    // Confirm which task is being removed
    cout << "Removing Task: [" << tasks[idNo - 1].id << "] "
         << tasks[idNo - 1].description << endl;

    // Shift all tasks after the deleted one back by one
    for (int i = idNo - 1; i < taskCounter - 1; ++i)
    {
        tasks[i] = tasks[i + 1];
        tasks[i].id = i + 1; // reassign correct IDs after shifting
    }

    taskCounter--; // reduce total count
    cout << "Task removed. :)\n";
}

/*
    showTaskList
    ------------
    Iterates through the array and displays all active tasks.
*/
void arrayHandler::showTaskList()
{
    // If no tasks exist
    if (taskCounter == 0)
    {
        cout << "Session Empty." << endl;
        return;
    }

    // Print all tasks one by one
    cout << "Session Task List:\n";
    for (int i = 0; i < taskCounter; i++)
    {
        cout << "[" << tasks[i].id << "] "
             << tasks[i].description
             << " (" << tasks[i].timeStamp << ")\n";
    }
    cout << endl;
}

/*
    getTaskCounter
    --------------
    Returns the current number of tasks stored.
*/
int arrayHandler::getTaskCounter()
{
    return taskCounter;
}

/*
    clearTask
    ---------
    Resets the task counter to zero, effectively clearing the session.
    (Note: Data remains in memory but is considered overwritten).
*/
void arrayHandler::clearTask()
{
    taskCounter = 0;
    cout << "Cleared History. ;)" << endl;
}