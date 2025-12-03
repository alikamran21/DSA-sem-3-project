#ifndef BTREE_INDEX_H
#define BTREE_INDEX_H

#include <string>
#include <iostream>
#include <stddef.h>

// Minimum degree T (T=3 is common, means 5 keys max, 6 children max)
#define T 3
#define MAX_KEYS (2 * T - 1)
#define MAX_CHILDREN (2 * T)

/*
 * BTreeNode
 */
struct BTreeNode {
    // **Fixed-size Array** of keys
    std::string keys[MAX_KEYS];

    // **Fixed-size Array** of child pointers
    BTreeNode* children[MAX_CHILDREN];

    int n; // Current number of keys
    bool leaf;

    BTreeNode(bool leaf_flag);
};

/*
 * BTreeIndex
 */
class BTreeIndex {
private:
    BTreeNode* root;

    void insertNonFull(BTreeNode* x, const std::string& k);
    void splitChild(BTreeNode* x, int i, BTreeNode* y);
    BTreeNode* searchNode(BTreeNode* x, const std::string& k);

public:
    BTreeIndex() : root(nullptr) {}
    void insert(const std::string& k);
    bool search(const std::string& k);

    void destroy(BTreeNode* node);
    ~BTreeIndex() { destroy(root); }
};

#endif
