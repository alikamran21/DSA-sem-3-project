#include "process_trie.h"
#include <cstring>   // For memset
#include <cctype>    // For tolower
#include <string>    // Standard string is still used for keys

using namespace std;

/*
    process_trie.cpp
    ----------------
    Implements a Trie (Prefix Tree) for efficient string searching.
    Optimized for storing and searching process names or malicious signatures.
    
    Complexity:
        - Insert: O(L) where L is length of string
        - Search: O(L)
*/

// --- TrieNode Implementation ---

// Constructor: Initializes the array of children pointers to nullptr
TrieNode::TrieNode() : isEndOfWord(false) {
    // Explicit use of C-style array initialization via memset
    memset(children, 0, sizeof(children));
}

// --- ProcessTrie Implementation ---

// Helper: Converts a character to the appropriate array index (0-25 for 'a'-'z')
int ProcessTrie::charToIndex(char c) {
    return tolower(c) - 'a';
}

ProcessTrie::ProcessTrie() {
    root = new TrieNode();
}

ProcessTrie::~ProcessTrie() {
    destroy(root);
}

/*
    destroy
    -------
    Recursively deletes all nodes in the Trie.
    Recursion ensures children are deleted before parents (post-order).
*/
void ProcessTrie::destroy(TrieNode* node) {
    if (!node) return;

    // Traverses the **Array of Pointers** to clean up children
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            destroy(node->children[i]);
        }
    }
    delete node;
}

/*
    insert
    ------
    Inserts a string key into the Trie.
    Creates new nodes for characters that don't exist yet in the path.
*/
void ProcessTrie::insert(const std::string& key) {
    TrieNode* curr = root;
    for (char c : key) {
        int index = charToIndex(c);
        if (index < 0 || index >= ALPHABET_SIZE) continue;

        if (!curr->children[index]) {
            // Dynamic allocation of a new node (Pointer usage)
            curr->children[index] = new TrieNode();
        }
        curr = curr->children[index];
    }
    curr->isEndOfWord = true;
}

/*
    searchPrefix
    ------------
    Checks if a given prefix exists in the Trie.
    Used for fast lookups (e.g., checking if "cmd" matches "cmd.exe").
*/
bool ProcessTrie::searchPrefix(const std::string& prefix) {
    TrieNode* curr = root;

    for (char c : prefix) {
        int index = charToIndex(c);
        if (index < 0 || index >= ALPHABET_SIZE) return false;

        if (!curr->children[index]) {
            return false;
        }
        curr = curr->children[index];
    }
    // If we reached the end of the prefix, the path exists
    return curr != nullptr;
}