#include "Circular_queue.h"
#include <iostream>
using namespace std;

// Constructor: this runs when a circularQueue object is created
// It sets up a circular buffer of a fixed size (given by 'size')
circularQueue::circularQueue(int size)
{
    capacity = size;                  // maximum number of actions that can be stored
    buffer = new Action[capacity];    // dynamic array to hold actions
    front = 0;                        // index of the first (oldest) element
    rear = -1;                        // index of the last (newest) element
    count = 0;                        // number of actions currently in the queue
}

// Destructor: cleans up memory that was manually allocated
circularQueue::~circularQueue()
{
    delete[] buffer;
}

// Function: isEmpty()
// Checks if there are no actions stored in the queue
bool circularQueue::isEmpty() const
{
    return count == 0;
}

// Function: isFull()
// Checks if the queue has reached its maximum storage capacity
bool circularQueue::isFull() const
{
    return count == capacity;
}

// Function: enqueue()
// Purpose: Add a new action to the queue
// If the queue is full, it automatically overwrites the oldest action
void circularQueue::enqueue(const Action &action)
{
    if (isFull())
    {
        // Overwrite oldest entry by moving both front and rear forward
        front = (front + 1) % capacity;
        rear = (rear + 1) % capacity;
        buffer[rear] = action;
    }
    else
    {
        // Normal case: just move rear forward and store new action
        rear = (rear + 1) % capacity;
        buffer[rear] = action;
        count++;
    }
}

// Function: dequeue()
// Purpose: Remove the oldest action from the queue
void circularQueue::dequeue()
{
    if (isEmpty())
    {
        cout << "Queue is Empty. Nothing to Remove :(\n";
        return;
    }

    cout << "Removing Oldest Action: " << buffer[front].description << endl;

    // Move front pointer forward (circularly)
    front = (front + 1) % capacity;
    count--;
}

// Function: peek()
// Purpose: Show (but not remove) the oldest action in the queue
Action circularQueue::peek() const
{
    if (isEmpty())
    {
        cout << "Queue is empty.\n";
        return {" ", " ", " "}; // return a dummy empty Action
    }
    return buffer[front];
}

// Function: displayAll()
// Purpose: Show all actions currently stored in the queue
void circularQueue::displayAll() const
{
    if (isEmpty())
    {
        cout << "No actions logged yet.\n";
        return;
    }

    cout << "Recent User Actions (" << count << " / " << capacity << "):\n";

    int index = front;
    for (int i = 0; i < count; ++i)
    {
        const Action &a = buffer[index];
        cout << "[" << i + 1 << "] User: " << a.user
             << " | Action: " << a.description
             << " | Time: " << a.timeStamp << endl;

        // Move to next action (circularly)
        index = (index + 1) % capacity;
    }

    cout << endl;
}

// Function: clear()
// Purpose: Completely reset the queue (delete all logs)
void circularQueue::clear()
{
    front = 0;
    rear = -1;
    count = 0;
    cout << "Logs Cleared.\n";
}
