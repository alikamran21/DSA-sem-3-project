#ifndef Circular_queue_h
#define Circular_queue_h

#include <iostream>
#include <string>
using namespace std;

struct Action
{
    string user;
    string description;
    string timeStamp;
};

class circularQueue
{
private:
    Action *buffer;
    int capacity;
    int front;
    int rear;
    int count;

public:
    circularQueue(int size = 10);
    ~circularQueue();
    void enqueue(const Action &action);
    void dequeue();
    Action peek() const;

    bool isEmpty() const;
    bool isFull() const;
    void displayAll() const;
    void clear();
};

#endif