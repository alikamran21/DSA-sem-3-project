#include "btree_index.h"
#include <cstring>
#include <string>
#include <algorithm> // For std::string comparison

using namespace std;

/*
    btree_index.cpp
    ---------------
    Implements a B-Tree data structure.
    Optimized for systems that read/write large blocks of data (like disk storage).
    
    Key DSA Concepts:
        - Multi-way Search Tree
        - Splitting full nodes
        - Key redistribution
*/

// --- BTreeNode Implementation ---

BTreeNode::BTreeNode(bool leaf_flag) : n(0), leaf(leaf_flag) {
    // Explicit use of C-style array initialization via memset
    memset(children, 0, sizeof(BTreeNode*) * MAX_CHILDREN);
}

// --- BTreeIndex Implementation ---

/*
    destroy
    -------
    Recursively deletes all nodes in the B-Tree.
    Used by the destructor to prevent memory leaks.
*/
void BTreeIndex::destroy(BTreeNode* node) {
    if (!node) return;

    for (int i = 0; i <= node->n; ++i) {
        if (node->children[i]) {
            destroy(node->children[i]);
        }
    }
    delete node;
}

/*
    insert
    ------
    Main interface to insert a key into the B-Tree.
    Handles the special case where the root is full and needs to split.
*/
void BTreeIndex::insert(const std::string& k) {
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        // If root is full, the tree must grow in height (disk optimization)
        if (root->n == MAX_KEYS) {
            BTreeNode* s = new BTreeNode(false);
            s->children[0] = root;

            // Split the full root
            splitChild(s, 0, root);

            // Determine which child (new or old) receives the key
            int i = 0;
            if (s->keys[0] < k) i++;
            insertNonFull(s->children[i], k);

            root = s;
        } else {
            insertNonFull(root, k);
        }
    }
}

/*
    splitChild
    ----------
    Splits a full child node (y) into two nodes.
    Promotes the median key to the parent node (x).
    Essential for keeping the B-Tree balanced.
*/
void BTreeIndex::splitChild(BTreeNode* x, int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->leaf);
    z->n = T - 1;

    // 1. Copy the last T-1 keys of y to z (using array index manipulation)
    for (int j = 0; j < T - 1; j++) {
        z->keys[j] = y->keys[j + T];
    }

    // 2. Copy the last T children of y to z, if y is not a leaf
    if (!y->leaf) {
        for (int j = 0; j < T; j++) {
            z->children[j] = y->children[j + T];
            y->children[j + T] = nullptr; // Clear pointers from y
        }
    }

    y->n = T - 1; // y now has T-1 keys

    // 3. Shift children pointers in x to make space for z (new child)
    for (int j = x->n; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }
    x->children[i + 1] = z;

    // 4. Shift keys in x to make space for the key promoted from y
    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
    }
    x->keys[i] = y->keys[T - 1]; // Promote median key
    x->n = x->n + 1;
}

/*
    insertNonFull
    -------------
    Inserts a key into a node that is not full.
    Traverses down the tree to finding the correct leaf position.
    Splits children on the way down if they are full.
*/
void BTreeIndex::insertNonFull(BTreeNode* x, const std::string& k) {
    int i = x->n - 1;

    if (x->leaf) {
        // Find location to insert key (while shifting larger keys right)
        while (i >= 0 && x->keys[i] > k) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->n = x->n + 1;
    } else {
        // Find the child which is going to have the new key
        while (i >= 0 && x->keys[i] > k) {
            i--;
        }
        i++;

        if (x->children[i]->n == MAX_KEYS) {
            splitChild(x, i, x->children[i]);

            if (x->keys[i] < k) {
                i++;
            }
        }
        insertNonFull(x->children[i], k); // **Recursion**
    }
}

// ======================= SEARCH IMPLEMENTATION =======================

/*
    searchNode
    ----------
    Recursively searches the tree for a key.
    Uses linear search within the node (could be optimized to binary search).
*/
BTreeNode* BTreeIndex::searchNode(BTreeNode* x, const std::string& k) {
    if (x == nullptr) return nullptr;

    int i = 0;
    while (i < x->n && k > x->keys[i]) {
        i++;
    }

    if (i < x->n && k == x->keys[i]) {
        return x;
    }

    if (x->leaf) {
        return nullptr;
    } else {
        return searchNode(x->children[i], k);
    }
}

/*
    search
    ------
    Public interface for searching a key.
    Returns true if found, false otherwise.
*/
bool BTreeIndex::search(const std::string& k) {
    // This search is O(log_T n), making it ideal for disk-based lookups.
    return searchNode(root, k) != nullptr;
}