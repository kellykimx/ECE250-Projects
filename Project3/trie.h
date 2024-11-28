#ifndef PROJECT3_TRIE_H
#define PROJECT3_TRIE_H
#include <iostream>
#include <string>
#include <vector>
#include "trie_node.h"

class Trie{
public:
    Trie();
    ~Trie();

    bool exitsign = true;//for exit function

    //Helper Functions
    void clean(TrieNode* node); //this will be used for destructor and clear function
    void printPrint(TrieNode* node, std::string currentPath, std::string& result);
    void invalid(const std::string& input);
    void updateTerminal(TrieNode* node);

    //COMMANDS
    void load(const std::string& filename);
    void insert(const std::string& classification);
    void classify(const std::string& input);
    void erase(const std::string& classification);
    void print();
    void empty();
    void clear();
    void size();
    bool exit();

private:
    TrieNode* root;
    int sizeTrie; //size of the trie
};

#endif //PROJECT3_TRIE_H
