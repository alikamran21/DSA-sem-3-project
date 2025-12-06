#include "graph_transition.h"
#include <algorithm>

/*
    graph_transition.cpp
    --------------------
    Implements a Graph using Adjacency Lists.
    Used to model valid transitions between user states (e.g., Login -> Work -> Logout).
    
    Algorithms:
        - BFS (Breadth-First Search)
        - DFS (Depth-First Search)
*/

GraphTransition::GraphTransition() {}

/*
    addTransition
    -------------
    Adds a directed edge from one state to another.
*/
void GraphTransition::addTransition(const string& fromState, const string& toState) {
    adjList[fromState].push_back(toState);
}

/*
    isValidTransition
    -----------------
    Checks if a direct edge exists between two states.
    Returns true if the transition is allowed.
*/
bool GraphTransition::isValidTransition(const string& fromState, const string& toState) {
    if (adjList.find(fromState) == adjList.end()) return false;
    
    const vector<string>& neighbors = adjList[fromState];
    for (const string& neighbor : neighbors) {
        if (neighbor == toState) return true;
    }
    return false;
}

/*
    bfs
    ---
    Performs Breadth-First Search starting from a node.
    Explores neighbors level-by-level using a Queue.
*/
void GraphTransition::bfs(const string& startNode) {
    if (adjList.find(startNode) == adjList.end()) {
        cout << "[BFS] Start node '" << startNode << "' not found in graph." << endl;
        return;
    }

    unordered_set<string> visited;
    queue<string> q;

    visited.insert(startNode);
    q.push(startNode);

    cout << "[BFS Traversal]: ";
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        cout << current << " -> ";

        // Visit all neighbors
        for (const string& neighbor : adjList[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    cout << "END" << endl;
}

/*
    dfs
    ---
    Performs Depth-First Search starting from a node.
    Explores paths deeply using a Stack.
*/
void GraphTransition::dfs(const string& startNode) {
    if (adjList.find(startNode) == adjList.end()) {
        cout << "[DFS] Start node '" << startNode << "' not found in graph." << endl;
        return;
    }

    unordered_set<string> visited;
    stack<string> s;

    s.push(startNode);

    cout << "[DFS Traversal]: ";
    while (!s.empty()) {
        string current = s.top();
        s.pop();

        if (visited.find(current) == visited.end()) {
            cout << current << " -> ";
            visited.insert(current);

            // Push neighbors to stack
            // We iterate in reverse to ensure the first neighbor is processed first (standard DFS behavior)
            if (adjList.find(current) != adjList.end()) {
                const vector<string>& neighbors = adjList[current];
                for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                    if (visited.find(*it) == visited.end()) {
                        s.push(*it);
                    }
                }
            }
        }
    }
    cout << "END" << endl;
}