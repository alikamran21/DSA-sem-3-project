#include "hash_profiles.h"

/*
    hash_profiles.cpp
    -----------------
    Implements a Hash Table with Chaining for collision resolution.
    Used for fast O(1) retrieval of user profiles by UserID.
*/

/*
    hashFunction
    ------------
    Computes a hash index for a given string key.
    Uses a polynomial rolling hash strategy.
*/
int hashTable::hashFunction(const string& key) const {
    long long hash = 0;
    for (char c : key) {
        hash = (hash * 37 + c) % capacity;
    }
    return (int)hash;
}

// Constructor
hashTable::hashTable(int size) {
    capacity = size;
    table = new hashNode*[capacity];

    for (int i = 0; i < capacity; i++)
        table[i] = nullptr;
}

// Destructor
hashTable::~hashTable() {
    for (int i = 0; i < capacity; i++) {
        hashNode* curr = table[i];
        while (curr) {
            hashNode* temp = curr;
            curr = curr->next;
            delete temp; 
            // Note: We do NOT delete profilePtr here as the AVL tree 
            // might be managed/allocated elsewhere in your main system.
        }
    }
    delete[] table;
}

/*
    addProfile
    ----------
    Inserts a new profile pointer into the hash table.
    Handles collisions using linked list chaining (add at head).
*/
void hashTable::addProfile(const string& userId, AVLProfile* profilePtr) {
    int index = hashFunction(userId);

    hashNode* newNode = new hashNode(userId, profilePtr);
    newNode->next = table[index];
    table[index] = newNode;

    cout << "[hashTable] Added profile for: " << userId << " at index " << index << endl;
}

/*
    getProfile
    ----------
    Retrieves the AVLProfile associated with a UserID.
    Returns nullptr if the user is not found.
*/
AVLProfile* hashTable::getProfile(const string& userId) {
    int index = hashFunction(userId);

    hashNode* curr = table[index];
    while (curr) {
        if (curr->userId == userId)
            return curr->profilePtr;
        curr = curr->next;
    }
    return nullptr;
}

/*
    displayTable
    ------------
    Prints the contents of the hash table (buckets and chains).
*/
void hashTable::displayTable() const {
    cout << "\n---- Hash Table ----\n";
    for (int i = 0; i < capacity; i++) {
        cout << "[" << i << "] ";
        hashNode* curr = table[i];
        while (curr) {
            cout << "(" << curr->userId << ") -> ";
            curr = curr->next;
        }
        cout << "NULL\n";
    }
}