#ifndef PROCESS_TRIE_H
#define PROCESS_TRIE_H

#include <string>
#include <stddef.h>

// Assuming process names use lowercase English letters (26)
#define ALPHABET_SIZE 26

/*
 * TrieNode
 */
struct TrieNode {
    // Array of Pointers to children (Explicit use of **Array of Pointers**)
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;

    TrieNode();
};

/*
 * ProcessTrie
 */
class ProcessTrie {
private:
    TrieNode* root;
    int charToIndex(char c);

public:
    ProcessTrie();
    ~ProcessTrie();

    // Insert a full process name (O(L))
    void insert(const std::string& key);

    // Check if a process name starts with a known malicious prefix
    bool searchPrefix(const std::string& prefix);

    // Helper for memory cleanup (uses **Recursion**)
    void destroy(TrieNode* node);
};

#endif
