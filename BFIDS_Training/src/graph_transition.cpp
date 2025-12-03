#include "graph_transition.h"
#include <algorithm>

GraphTransition::GraphTransition() {}

void GraphTransition::addTransition(const string& fromState, const string& toState) {
    adjList[fromState].push_back(toState);
}

bool GraphTransition::isValidTransition(const string& fromState, const string& toState) {
    if (adjList.find(fromState) == adjList.end()) return false;
    
    const vector<string>& neighbors = adjList[fromState];
    for (const string& neighbor : neighbors) {
        if (neighbor == toState) return true;
    }
    return false;
}

// --- BFS Implementation (Queue Based) ---
// Used to explore reachable states level-by-level (e.g., Immediate threats vs distant threats)
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

// --- DFS Implementation (Stack Based) ---
// Used to explore deep transition paths (e.g., verifying if a fatal state is reachable)
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