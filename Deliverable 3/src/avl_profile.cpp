#include "../include/avl_profile.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

/*
    avl_profile.cpp
    ----------------
    This file stores the behavioral fingerprint using an AVL tree.
    Each node holds:
        - process name
        - how often the user uses it (frequency)
        - the average time they spend using it (avgDuration)

    During training:
        insertOrUpdate() is used for every UserAction.
        If the process already exists, we update frequency and average.
        If it does not exist, we create a new node.

    During live detection:
        search() is called to look up the expected pattern.
        The detection logic will compare:
            live frequency vs trained frequency
            live avgDuration vs trained avgDuration
        Any unusual difference can be flagged as anomaly.

    The tree is self-balancing using AVL rotations.
*/

// Constructor / Destructor
AVLProfile::AVLProfile() : root(nullptr) {}

AVLProfile::~AVLProfile() {
    destroy(root);
}

// Delete nodes recursively
void AVLProfile::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// Basic height function
int AVLProfile::height(Node* n) const {
    return n ? n->height : 0;
}

// Balance factor
int AVLProfile::getBalance(Node* n) const {
    if (!n) return 0;
    return height(n->left) - height(n->right);
}

/*
    Standard AVL rotation functions.
    These keep the tree balanced for fast search.
*/
AVLProfile::Node* AVLProfile::rightRotate(Node* y) {
    Node* x = y->left;
    Node* t = x->right;

    x->right = y;
    y->left = t;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

AVLProfile::Node* AVLProfile::leftRotate(Node* x) {
    Node* y = x->right;
    Node* t = y->left;

    y->left = x;
    x->right = t;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

/*
    insertOrUpdate()
    ----------------
    Called during training.
    - If process does not exist: create a new node.
    - If it exists: update frequency and running average.
*/
void AVLProfile::insertOrUpdate(const std::string& processName, double duration) {
    root = insertOrUpdateNode(root, processName, duration);
}

AVLProfile::Node* AVLProfile::insertOrUpdateNode(Node* node, const string& processName, double duration) {

    // Standard BST insert
    if (!node) {
        UserActionProfile p(processName, 1, duration);
        return new Node(p);
    }

    if (processName < node->profile.processName) {
        node->left = insertOrUpdateNode(node->left, processName, duration);
    }
    else if (processName > node->profile.processName) {
        node->right = insertOrUpdateNode(node->right, processName, duration);
    }
    else {
        // Update existing process stats
        double total = node->profile.avgDuration * node->profile.frequency + duration;
        node->profile.frequency++;
        node->profile.avgDuration = total / node->profile.frequency;
        return node;
    }

    // Update height after insertion
    node->height = 1 + max(height(node->left), height(node->right));

    // Balance the tree
    int balance = getBalance(node);

    if (balance > 1 && processName < node->left->profile.processName)
        return rightRotate(node);

    if (balance < -1 && processName > node->right->profile.processName)
        return leftRotate(node);

    if (balance > 1 && processName > node->left->profile.processName) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && processName < node->right->profile.processName) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

/*
    insertProfileDirect()
    ---------------------
    Used when loading fingerprint from disk.
*/
void AVLProfile::insertProfileDirect(const UserActionProfile& profile) {
    root = insertProfileNode(root, profile);
}

AVLProfile::Node* AVLProfile::insertProfileNode(Node* node, const UserActionProfile& profile) {
    if (!node) return new Node(profile);

    if (profile.processName < node->profile.processName)
        node->left = insertProfileNode(node->left, profile);
    else if (profile.processName > node->profile.processName)
        node->right = insertProfileNode(node->right, profile);
    else {
        node->profile = profile;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && profile.processName < node->left->profile.processName)
        return rightRotate(node);

    if (balance < -1 && profile.processName > node->right->profile.processName)
        return leftRotate(node);

    if (balance > 1 && profile.processName > node->left->profile.processName) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && profile.processName < node->right->profile.processName) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

/*
    Search for a process during live detection.
*/
AVLProfile::Node* AVLProfile::findNode(Node* node, const string& processName) const {
    if (!node) return nullptr;
    if (processName == node->profile.processName) return node;
    if (processName < node->profile.processName) return findNode(node->left, processName);
    return findNode(node->right, processName);
}

bool AVLProfile::search(const string& processName, UserActionProfile& outProfile) const {
    Node* n = findNode(root, processName);
    if (!n) return false;
    outProfile = n->profile;
    return true;
}

/*
    Export and import functions for saving fingerprint to CSV.
*/
void AVLProfile::inorderExport(Node* node, const function<void(const UserActionProfile&)>& fn) const {
    if (!node) return;
    inorderExport(node->left, fn);
    fn(node->profile);
    inorderExport(node->right, fn);
}

bool AVLProfile::exportToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "ProcessName,Frequency,AvgDuration\n";

    inorderExport(root, [&](const UserActionProfile& p) {
        file << p.processName << "," << p.frequency << "," << p.avgDuration << "\n";
    });

    return true;
}

bool AVLProfile::importFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if (line.empty()) continue;

        string name;
        int freq;
        double avg;

        stringstream ss(line);
        getline(ss, name, ',');
        ss >> freq;
        ss.ignore(1, ',');
        ss >> avg;

        UserActionProfile p(name, freq, avg);
        insertProfileDirect(p);
    }

    return true;
}
