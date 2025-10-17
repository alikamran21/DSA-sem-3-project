#ifndef Circular_queue_h
#define Circular_queue_h

#include <iostream>
#include <string>
using namespace std;

struct Action {
    string user;
    string description;
    string timeStamp;
};

class circularqueue{
private:
    Action* buffer;
    int capacity;
    int front;
    int rear;
    int count;
public:
    circularqueue(int size = 10);
    ~circularqueue();
    void enqueue (const Action& action);
    void dequeue ();
    Action peek() const;

    bool isEmpty();
    bool isFull();
    void displayAll();
    void clear();
};

#endif 