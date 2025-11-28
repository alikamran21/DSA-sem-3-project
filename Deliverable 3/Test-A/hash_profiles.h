#ifndef HASH_PROFILES_H
#define HASH_PROFILES_H

#include <string>
#include <iostream>

// Include the AVL Profile header.
// This path assumes hash_profiles is in 'Test-A' and avl_profile is in 'Test-F/include'
#include "../Test-F/include/avl_profile.h"

using namespace std;

class hashNode {
public:
    string userId;
    AVLProfile* profilePtr; // Corrected type: AVLProfile*
    hashNode* next;

    // Constructor: Takes a pointer to an AVLProfile
    hashNode(const string& id, AVLProfile* ptr) 
        : userId(id), profilePtr(ptr), next(nullptr) {}
};

class hashTable {
private:
    hashNode** table; // Corrected: Array of pointers to hashNodes
    int capacity;
    
    int hashFunction(const string& key) const;

public:
    hashTable(int size = 20);
    ~hashTable();
    
    // Add a profile pointer for a specific User ID
    void addProfile(const string& userId, AVLProfile* profilePtr);
    
    // Retrieve the profile pointer
    AVLProfile* getProfile(const string& userId);

    void displayTable() const;
};

#endif