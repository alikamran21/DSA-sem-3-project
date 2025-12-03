#ifndef GRAPH_TRANSITION_H
#define GRAPH_TRANSITION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

class GraphTransition {
private:
    // Adjacency list: State -> List of next possible States
    unordered_map<string, vector<string>> adjList;

public:
    GraphTransition();
    
    // Core Graph Operations
    void addTransition(const string& fromState, const string& toState);
    bool isValidTransition(const string& fromState, const string& toState);
    
    // Traversal Algorithms (New Implementation)
    void bfs(const string& startNode); // Breadth-First Search
    void dfs(const string& startNode); // Depth-First Search
};

#endif