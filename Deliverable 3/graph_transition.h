#ifndef GRAPH_TRANSITION_H
#define GRAPH_TRANSITION_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Edge {
    string from;
    string to;
    Edge* next;

    Edge(const string& f, const string& t)
        : from(f), to(t), next(nullptr) {}
};

class GraphTransition {
private:
    Edge** adjacencyList;  
    int bucketCount;

    int hash(const string& key) const;

public:
    GraphTransition(int buckets = 30);
    ~GraphTransition();

    void addTransition(const string& from, const string& to);

    bool isValidTransition(const string& from, const string& to);

    void displayGraph() const;
};

#endif
