#ifndef HASH_PROFILES_H
#define HASH_PROFILES_H

#include <string>
#include <iostream>
using namespace std;

// #include "add avl tree filew name"

class hashNode
{

public:
    string userId;
    avlProfile *profilePtr; // to be fixed on avl file being added
    hashNode *next;
    hashNode(const string &id, avlProfile &ptr) : userId(id), profilePtr(ptr), next(nullptr) {} // to be fixed on avl file being added
};

class hashTable
{
private:
    hashTable **table;
    int capacity;
    int hashFunction(const string &key) const;

public:
    hashTable(int size = 20);
    ~hashTable();
    
    void addProfile(const string &userId, AvlProfile *profilePtr);
    AvlProfile *getProfile(const string &userId); // to be corrected on avlfile being made

    void displayTable() const;
};

#endif