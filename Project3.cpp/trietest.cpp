#include "ece250_socket.h"
#include "trie.h"
#include "trie_node.h"
#include "illegal.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

int main(){
    Trie* trie = new Trie();
    std::string answer;
    std::string filename;
    std::string classification;
    std::string input;

    while (trie->exitsign) {
        std::cin>>answer;
        try {
            if (answer == "LOAD") {
                std::cin>>filename;
                trie->load(filename);
            } else if (answer == "INSERT") {
                std::cin.ignore();
                std::getline(std::cin, classification);
                classification.erase(0, classification.find_first_not_of(' '));
                trie->insert(classification);
            } else if (answer == "CLASSIFY") {
                std::cin.ignore();
                std::getline(std::cin, input);
                input.erase(0, input.find_first_not_of(' '));
                trie->classify(input);
            } else if (answer == "ERASE") {
                std::cin.ignore();
                std::getline(std::cin, classification);
                classification.erase(0, classification.find_first_not_of(' '));
                trie->erase(classification);
            } else if (answer == "PRINT") {
                trie->print();
            } else if (answer == "EMPTY") {
                trie->empty();
            } else if (answer == "CLEAR") {
                trie->clear();
            } else if (answer == "SIZE") {
                trie->size();
            } else if (answer == "EXIT") {
                trie->exit();
            }
        } catch (illegal_exception& e) {
            std::cout<<e.what()<<std::endl; //print illegal argument
        }
    }
    delete trie;
    return 0;
}
