#include "hash_profiles.h"

int hashTable :: hashFunction(const string& key)const{
    long long hash = 0;
    for (char c : key)
    {
        hash = (hash  * 37 + c ) % capacity ;
        return (int)hash;
    }
    
}
hashTable::hashTable(int size) {
    capacity = size;
    table = new hashNode*[capacity];

    for (int i = 0; i < capacity; i++)
        table[i] = nullptr;
}

hashTable::~hashTable() {
    for (int i = 0; i < capacity; i++) {
        hashNode* curr = table[i];
        while (curr) {
            hashNode* temp = curr;
            curr = curr->next;
            delete temp;     
        }
    }
    delete[] table;
}

void hashTable::addProfile (cosnt string& userId , AvlProfile* profilePtr){
    int index = hashFunction(userId);

    hashNode* newNode = new hashNode(userId , profilePtr);
    newNode->next = table[index];
    table[index] = newNode;

    cout << "[hashTable] added profile for: " userId << " at index " << index << endl;


}
AVLProfile* HashTable::getProfile(const string& userID) {
    int index = hashFunction(userID);

    HashNode* curr = table[index];
    while (curr) {
        if (curr->userID == userID)
            return curr->profilePtr;
        curr = curr->next;
    }
    return nullptr;
}

void HashTable::displayTable() const {
    cout << "\n---- Hash Table ----\n";
    for (int i = 0; i < capacity; i++) {
        cout << "[" << i << "] ";
        HashNode* curr = table[i];
        while (curr) {
            cout << "(" << curr->userID << ") -> ";
            curr = curr->next;
        }
        cout << "NULL\n";
    }
}