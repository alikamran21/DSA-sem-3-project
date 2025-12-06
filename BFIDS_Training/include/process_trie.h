#ifndef PROCESS_TRIE_H
#define PROCESS_TRIE_H

#include <string>
#include <stddef.h>

// Assuming process names use lowercase English letters (26)
#define ALPHABET_SIZE 26

/*
 * ProcessTrie
 * -----------
 * Trie data structure for efficient prefix searching of process names.
 * Provides:
 * - insert(): adds a full process name
 * - searchPrefix(): checks for known malicious prefixes
 * - destroy(): recursively clears memory
 *
 * Node stores:
 * TrieNode { children array, isEndOfWord flag }
 */

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