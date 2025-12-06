#include "../include/avl_profile.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include "../include/btree_index.h"
using namespace std;

/*
    avl_profile.cpp
    ---------------
    Implementation of the AVL Tree for behavioral fingerprinting.
    
    This file handles the storage, update, and retrieval of user behavior statistics.
    It ensures the tree remains balanced (AVL property) for consistent O(log n) performance,
    which is critical when checking thousands of actions against a profile.
    
    DSA Concepts:
        - Binary Search Tree (BST) operations
        - AVL Rotations (LL, RR, LR, RL)
        - Recursion (Insert, Search, Traversal)
*/

// Helper to save B-Tree index (Disk optimization simulation)
void saveBTreeIndexToDisk(BTreeIndex* index, const std::string& indexFilename) {
    std::cout << "[B-Tree Utility] Saving index to " << indexFilename << "..." << std::endl;
}

// Helper to load B-Tree index
BTreeIndex* loadBTreeIndexFromDisk(const std::string& indexFilename) {
    std::cout << "[B-Tree Utility] Loading index from " << indexFilename << "..." << std::endl;
    return new BTreeIndex();
}

// Constructor
AVLProfile::AVLProfile() : root(nullptr) {}

// Destructor
AVLProfile::~AVLProfile() {
    destroy(root);
}

/*
    destroy
    -------
    Recursively deletes all nodes in the tree.
    Traverses in post-order (left, right, root) to safely free memory.
*/
void AVLProfile::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

/*
    height
    ------
    Returns the height of a given node.
    Handles nullptr safely (height 0).
*/
int AVLProfile::height(Node* n) const {
    return n ? n->height : 0;
}

/*
    getBalance
    ----------
    Calculates the balance factor of a node (left height - right height).
    Used to detect if a rotation is needed.
*/
int AVLProfile::getBalance(Node* n) const {
    if (!n) return 0;
    return height(n->left) - height(n->right);
}

/*
    rightRotate
    -----------
    Performs a Right Rotation on the subtree rooted at y.
    Used to fix Left-Left cases.
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

/*
    leftRotate
    ----------
    Performs a Left Rotation on the subtree rooted at x.
    Used to fix Right-Right cases.
*/
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
    insertOrUpdate
    --------------
    Public interface for training data.
    If the process exists, it updates the stats.
    If not, it inserts a new node.
*/
void AVLProfile::insertOrUpdate(const std::string& processName, double duration) {
    root = insertOrUpdateNode(root, processName, duration);
}

/*
    insertOrUpdateNode
    ------------------
    Recursive function to insert or update a node.
    Calculates the new running average for duration if the node exists.
    Performs AVL balancing after insertion.
*/
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
        // Update existing process stats (Moving Average Calculation)
        double total = node->profile.avgDuration * node->profile.frequency + duration;
        node->profile.frequency++;
        node->profile.avgDuration = total / node->profile.frequency;
        return node;
    }

    // Update height after insertion
    node->height = 1 + max(height(node->left), height(node->right));

    // Balance the tree
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && processName < node->left->profile.processName)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && processName > node->right->profile.processName)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && processName > node->left->profile.processName) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && processName < node->right->profile.processName) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

/*
    insertProfileDirect
    -------------------
    Public interface for loading existing profiles.
    Does not update averages; simply places the data into the tree.
*/
void AVLProfile::insertProfileDirect(const UserActionProfile& profile) {
    root = insertProfileNode(root, profile);
}

/*
    insertProfileNode
    -----------------
    Recursive function to insert a pre-defined profile node.
    Maintains AVL balance.
*/
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
    findNode
    --------
    Recursively searches for a process name in the tree.
    Returns the node pointer if found, nullptr otherwise.
*/
AVLProfile::Node* AVLProfile::findNode(Node* node, const string& processName) const {
    if (!node) return nullptr;
    if (processName == node->profile.processName) return node;
    if (processName < node->profile.processName) return findNode(node->left, processName);
    return findNode(node->right, processName);
}

/*
    search
    ------
    Public interface to search for a process profile.
    Populates outProfile with the found data if successful.
*/
bool AVLProfile::search(const string& processName, UserActionProfile& outProfile) const {
    Node* n = findNode(root, processName);
    if (!n) return false;
    outProfile = n->profile;
    return true;
}

/*
    inorderExport
    -------------
    Traverses the tree in-order (sorted by process name) and applies a function.
    Used to write data to CSV sequentially.
*/
void AVLProfile::inorderExport(Node* node, const function<void(const UserActionProfile&)>& fn) const {
    if (!node) return;
    inorderExport(node->left, fn);
    fn(node->profile);
    inorderExport(node->right, fn);
}

/*
    exportToCSV
    -----------
    Saves the entire tree structure to a CSV file.
    Also builds a temporary B-Tree index for demonstration.
*/
bool AVLProfile::exportToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;
    BTreeIndex profileIndex;

    file << "ProcessName,Frequency,AvgDuration\n";

    inorderExport(root, [&](const UserActionProfile& p) {
        file << p.processName << "," << p.frequency << "," << p.avgDuration << "\n";
        profileIndex.insert(p.processName);
    });
    saveBTreeIndexToDisk(&profileIndex, "fingerprint_index.bin");
    return true;
}

/*
    importFromCSV
    -------------
    Loads profile data from a CSV file into the AVL tree.
    Simulates checking a B-Tree index first for optimization.
*/
bool AVLProfile::importFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    
    // Check secondary index first (simulation)
    BTreeIndex* profileIndex = loadBTreeIndexFromDisk("fingerprint_index.bin");
    if (profileIndex && profileIndex->search("cmd.exe")) {
        cout << "[B-Tree] Profile for 'cmd.exe' found quickly via disk index ." <<endl;
    } else {
        cout << "[B-Tree] Index not found or key not present. Proceeding with full CSV scan." << endl;
    }
    
    string line;
    getline(file, line); // Skip header

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
    if (profileIndex) {
        delete profileIndex;
    }
    return true;
}