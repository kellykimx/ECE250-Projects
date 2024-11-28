#ifndef PROJECT3_TRIE_NODE_H
#define PROJECT3_TRIE_NODE_H

#include <string>
#include <vector>
#include <iostream>
#include "trie.h"

class TrieNode{
public:
    TrieNode();
    ~TrieNode();

    bool terminal; //checking if the node is terminal or not
    std::string className;
    std::vector<TrieNode*> children;
};

#endif //PROJECT3_TRIE_NODE_H
