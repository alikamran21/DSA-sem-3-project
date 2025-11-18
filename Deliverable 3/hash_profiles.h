#ifndef HASH_PROFILE_H
#define HASH_PROFILE_H

#include <string>
#include <iostream>
using namespace std;

// #include "add avl tree filew name"

class hashNode
{

public:
    string userId;
    avlProfile profile;
    hashNode* next;
    hashNode(const string &id) : userId(id) , next(nullptr) {}
};

class hashTable
{
private:
    hashTable** table;
    int capacity;
    int hashFunction (const string &key) const;
public:
    hashTable(int size = 20);
    ~hashTable();

    void registerUser (const string& userId);
    // AvlProfile* getProfile(const string& userId);  to be corrected on avlfile being made

    void updateProfile(const string& userId, const string command , double duration);

    void matchProfile(const string& userId, const string command, double duration );

    void displayTable();

};


#endif
