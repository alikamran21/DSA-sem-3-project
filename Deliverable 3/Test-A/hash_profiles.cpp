#include "hash_profiles.h"

// Hash Function
// FIXED: Now loops through the entire string before returning
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
            // If the hash table owns the memory, add: delete temp->profilePtr;
        }
    }
    delete[] table;
}

// Add Profile
// FIXED: 'const' typo and missing '<<' in cout
void hashTable::addProfile(const string& userId, AVLProfile* profilePtr) {
    int index = hashFunction(userId);

    hashNode* newNode = new hashNode(userId, profilePtr);
    newNode->next = table[index];
    table[index] = newNode;

    cout << "[hashTable] Added profile for: " << userId << " at index " << index << endl;
}

// Get Profile
// FIXED: Correct return type and comparison logic
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

// Display Table
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