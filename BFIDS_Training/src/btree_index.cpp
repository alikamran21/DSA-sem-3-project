#include "btree_index.h"
#include <cstring>
#include <string>
#include <algorithm> // For std::string comparison

using namespace std;

// --- BTreeNode Implementation ---

BTreeNode::BTreeNode(bool leaf_flag) : n(0), leaf(leaf_flag) {
    // Explicit use of C-style array initialization via memset
    memset(children, 0, sizeof(BTreeNode*) * MAX_CHILDREN);
}

// --- BTreeIndex Implementation ---

/*
    Purpose: Cleans up all dynamically allocated BTreeNodes.
    DSA Concept: Utilizes **Recursion** and **Pointers** for deep memory cleanup.
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
    Purpose: Inserts a new key (process name) into the B-Tree.
    DSA Concept: Main function for B-Tree insertion, handles root splitting.
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
    Purpose: Splits a full child node (y) of a non-full parent (x).
    DSA Concept: Core B-Tree operation using fixed-size **Arrays** to manage keys and pointers.
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
    Purpose: Inserts a key into a node that is guaranteed not to be full.
    DSA Concept: Traverses down the tree using **Recursion** until a leaf is found or a split is triggered.
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
    Purpose: Recursively searches for the node containing key 'k'.
    DSA Concept: **Recursion** and comparison on the array of keys in each node.
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

bool BTreeIndex::search(const std::string& k) {
    // This search is O(log_T n), making it ideal for disk-based lookups.
    return searchNode(root, k) != nullptr;
}