#include "Circular_queue.h"
#include <iostream>

circularQueue::circularQueue(int size)
{
    capacity = size;
    buffer = new Action[capacity];
    front = 0;
    rear = -1;
    count = 0;
}

circularQueue::~circularQueue()
{
    delete[] buffer;
}

bool circularQueue::isEmpty() const
{
    return count == 0;
}
bool circularQueue::isFull() const
{
    return count == capacity;
}
void circularQueue::enqueue(const Action &action)
{
    if (isFull())
    {
        front = (front + 1) % capacity;
        rear = (rear + 1) % capacity;
        buffer[rear] = action;
    }
    else
    {
        rear = (rear + 1) % capacity;
        buffer[rear] = action;
        count++;
    }
}

void circularQueue::dequeue()
{
    if (isEmpty())
    {
        cout << "Queue is Empty. Nothing to Remove :(\n";
        return;
    }
    cout << "removing Oldest Action: " << buffer[front].description << endl;
    front = (front + 1) % capacity;
    count--;
}
Action circularQueue::peek() const
{
    if (isEmpty())
    {
        cout << "Queue is empty: \n";
        return {" ", " ", " "};
    }
    return buffer[front];
}

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
        index = (index + 1) % capacity;
    }
    cout << endl;
}

void circularQueue::clear()
{
    front = 0;
    rear = -1;
    count = 0;
    cout << "Logs Cleared.\n";
}
