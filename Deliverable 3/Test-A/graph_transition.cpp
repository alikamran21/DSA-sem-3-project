#include "graph_transition.h"

int GraphTransition::hash(const string& key) const {
    long long h = 0;
    for (char c : key)
        h = (h * 31 + c) % bucketCount;
    return (int)h;
}

GraphTransition::GraphTransition(int buckets) {
    bucketCount = buckets;
    adjacencyList = new Edge*[bucketCount];

    for (int i = 0; i < bucketCount; i++)
        adjacencyList[i] = nullptr;
}

GraphTransition::~GraphTransition() {
    for (int i = 0; i < bucketCount; i++) {
        Edge* e = adjacencyList[i];
        while (e) {
            Edge* temp = e;
            e = e->next;
            delete temp;
        }
    }
    delete[] adjacencyList;
}

void GraphTransition::addTransition(const string& from, const string& to) {
    int index = hash(from);

    Edge* curr = adjacencyList[index];
    while (curr) {
        if (curr->from == from && curr->to == to)
            return; 
        curr = curr->next;
    }

    Edge* newEdge = new Edge(from, to);
    newEdge->next = adjacencyList[index];
    adjacencyList[index] = newEdge;

    cout << "[Graph] Learned transition: " << from << " → " << to << endl;
}

bool GraphTransition::isValidTransition(const string& from, const string& to) {
    int index = hash(from);

    Edge* curr = adjacencyList[index];
    while (curr) {
        if (curr->from == from && curr->to == to)
            return true;
        curr = curr->next;
    }

    return false;
}

void GraphTransition::displayGraph() const {
    cout << "\n--- Transition Graph ---\n";
    for (int i = 0; i < bucketCount; i++) {
        Edge* curr = adjacencyList[i];
        if (!curr) continue;
        cout << "[" << i << "]: ";
        while (curr) {
            cout << "(" << curr->from << " → " << curr->to << ") ";
            curr = curr->next;
        }
        cout << endl;
    }
}