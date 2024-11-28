#include "trie.h"
#include "trie_node.h"
#include "illegal.h"
#include "ece250_socket.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

Trie::Trie(){ //constructor for trie
    sizeTrie=0;
    root = new TrieNode(); //
}

Trie::~Trie(){ //destructor for trie
    if (root != nullptr){
        clean(root);
    }
}
TrieNode::TrieNode() { //constructor for trienode
    terminal = true;
    //N=15 (fixed size of children), nullptr is the initial value for all vectors since there are no children
    //children member of TrieNode objects
    children = std::vector<TrieNode*>(15, nullptr); //holds pointers to TrieNode objects
}
TrieNode::~TrieNode() { //destructor for trienode
    //no need for this, since trie destructor already cleans up
}

//HELPER FUNCTIONS
void Trie::clean(TrieNode* node){
    if (node == nullptr) {
        return; //if the node is already null, nothing has to be done
    }
    //deleting the child node of the current node and pointers are set to nullptr
    for (size_t i = 0; i < node->children.size(); i++){
        if (node->children[i] != nullptr) { //checking if the ith child node of node exists
            clean(node->children[i]); //recursively deletes the node
            node->children[i]=nullptr; //setting it to nullptr
        }
    }
    delete node;
}

void Trie::printPrint(TrieNode* node, std::string path, std::string& output) {
    if (node == nullptr) { //if the node is nullptr, there is nothing to print
        return;
    }

    // Only append the current path if the node is terminal and has no children
    bool hasChildren = false;
    for (size_t i = 0; i < 15; i++) {
        if (node->children[i] != nullptr) { //if the node has any children, it breaks out of the loop
            hasChildren = true; //sets the bool to true
            break;
        }
    }

    if (node->terminal && !hasChildren) { //if the node is terminal and has no children
        if (!output.empty()) { //if the return value (output) is not empty, add an underscore to separate
            output += "_"; // add a separator between paths
        }
        output += path; //then add the current path
    }

    // Recurse for all children
    for (size_t i = 0; i < 15; i++) { //iterating through possible 15 children nodes
        TrieNode* child = node->children[i];
        if (child != nullptr) {
            //if the path is empty, set newPath to child's className or add className to path using comma to separate
            std::string newPath = path.empty() ? child->className : path + "," + child->className;
            printPrint(child, newPath, output); // Recursive call
        }
    }
}
//this is for the illegal_exception, checking its validity
void Trie::invalid(const std::string& input) {
    for (char c: input){
        if(std::isupper(c)){ //if it includes any upper case letters
            throw illegal_exception(); //"illegal argument"
        }
    }
}

//COMMANDS
void Trie::load(const std::string& filename){ //DONE
    std::ifstream file(filename);
    std::string line; //line stores each line of file

    //reads every line of code
    while (std::getline(file, line)) {
        TrieNode* start = root; //starting from the root
        std::stringstream classificationStream(line); //splitting the line into individual class names
        std::string className;
        std::vector<TrieNode*> path; //this is to track visited node

        //this is to check that we are not adding duplicates
        while (std::getline(classificationStream, className, ',')) {
            bool found = false; //to indicate if there is a duplicate
            //checking through 15 possible children
            for (size_t i = 0; i < 15; i++) {
                if (start->children[i] != nullptr && start->children[i]->className == className) {
                    path.push_back(start);
                    start = start->children[i];
                    found = true;
                    break;
                }
            }
            // If it does not exist, create a new node
            if (found==false) {
                for (size_t i = 0; i < 15; i++) {
                    if (start->children[i] == nullptr) {
                        TrieNode* newNode = new TrieNode();
                        newNode->className = className;
                        start->children[i] = newNode;
                        path.push_back(start);
                        start = newNode;
                        sizeTrie++; //increasing the size of the trie
                        break;
                    }
                }
            }
        }
        // Mark the last node as terminal if it's not already
        if (!start->terminal) {
            start->terminal = true;
            sizeTrie++;
        }
        //updating all nodes in the trie (if it is terminal or not)
        for (TrieNode* node : path) {
            bool hasTerminalChild = false;

            // Check if any child is terminal
            for (size_t i = 0; i < 15; i++) {
                if (node->children[i] != nullptr && node->children[i]->terminal) {
                    hasTerminalChild = true;
                    break;
                }
            }

            // Unmark the node as terminal if it has terminal children
            if (hasTerminalChild) {
                node->terminal = false;
            }
        }
    }
    file.close();
    //outputting success no matter what
    std::cout << "success" << std::endl;
}

void Trie::insert(const std::string& classification) { //DONE
    //illegal exceptions, handling illegal inputs
    try {
        invalid(classification);
    } catch (illegal_exception& e) {
        std::cout<<e.what()<<std::endl;
        return;
    }

    //actually inserting
    TrieNode* start = root; //starting from the root
    std::stringstream classificationStream(classification); //splits input classification into individual class names
    std::string className; //storing each className
    bool isNewNodeAdded = false; // to track if new node was added

    while(std::getline(classificationStream, className, ',')){
        bool found=false; //checking if the className exists as a child node of start (current node)
        for (size_t i=0; i<15; i++){ //N is fixed (15)
            //if the current node's child does not equal to nullptr, and the className is equal to className
            if ((start->children[i] != nullptr) && (start->children[i]->className == className)) {
                //if it does match, change the current pointer (start) to its child, and continue travelling down the path
                start = start->children[i];
                found=true;
                break;
            }
        }
        //if the classification was not found
        if (found == false) {
            for (size_t i=0; i<15; i++) { //going through the child node
                if (start->children[i] == nullptr){
                    TrieNode* newNode = new TrieNode(); //create new node
                    newNode->className = className; //make that nodes className to className
                    start->children[i]=newNode; //set the node's child to new node
                    start = newNode; //now make that node set to current node
                    isNewNodeAdded = true; // Mark that a new node was added
                    sizeTrie++;
                    break;
                }
            }
        }
    }
    TrieNode* current = root;
    std::stringstream pathStream(classification);
    while (std::getline(pathStream, className, ',')) {
        for (size_t i = 0; i < 15; i++) {
            if (current->children[i] != nullptr && current->children[i]->className == className) {
                current = current->children[i];
                break;
            }
        }
        // If the current node is an ancestor and has terminal children, mark it as non-terminal
        if (current != start && current->terminal) {
            current->terminal = false;
        }
    }

    // Output result
    if (isNewNodeAdded) {
        std::cout << "success" << std::endl;
    } else {
        std::cout << "failure" << std::endl; // The classification already exists
    }
}

void Trie::classify(const std::string& input) {
    try {
        invalid(input); // Check for illegal input
    } catch (illegal_exception& e) {
        std::cout << e.what() << std::endl; //"illegal argument"
        return;
    }

    TrieNode* start = root; //starting at the root
    std::string path=""; //adds the labels of the classification hierarchy as function travels down
    std::string checkInput = input;

    while(true) {
        std::string candidateLabel;

        for (size_t i = 0; i < start->children.size(); i++) {
            if (start->children[i] != nullptr) {  //if the child exists, add the className to candidate
                if (i > 0) {
                    candidateLabel += ",";
                }
                candidateLabel += start->children[i]->className;
            }
        }
        std::string next = labelText(checkInput, candidateLabel);
        if (path.empty() == false) { //if the path is not empty, add a comma
            path += ",";
        }
        path += next; //then add next to the path
        TrieNode* nextNode = nullptr;
        for (TrieNode* child: start->children) {  //iterating through childrne to find which child matches up with next
            if ((child != nullptr) && (child->className == next)) {
                nextNode = child; //points to the matching child
                break;
            }
        }

        if(nextNode->terminal==true) { //if the node is terminal, exits the loop
            break;
        }
        start = nextNode; //if not terminal, it keeps going through the loop
    }
    std::cout<<path<<std::endl;
}

void Trie::erase(const std::string& classification) { //DONE
    try {
        invalid(classification);
    } catch (illegal_exception& e) {
        std::cout<<e.what()<<std::endl; //"illegal argument"
        return;
    }
    //if the trie is empty, then print failure
    if (root == nullptr || sizeTrie == 0) {
        // If the trie is empty
        std::cout << "failure" << std::endl;
        return;
    }
    TrieNode* start = root; //starting from the root
    std::stringstream classificationStream(classification);
    std::vector<TrieNode*> path; // store the path of nodes for tracking
    std::vector<int> indexfound;   // Store the index of children for cleanup
    std::string className;

    while (std::getline(classificationStream, className, ',')) {
        bool found = false; //to check if the classification is found
        for (size_t i=0; i<15; i++){
            if ((start->children[i] != nullptr) && (start->children[i]->className == className)) {
                found=true;
                path.push_back(start); //add the node to path, so we can come back to it (to clean up), storing the path
                indexfound.push_back(i); //this is to indicate what child needs to be deleted
                start=start->children[i];
                break;
            }
        }
        if (found == false) { //if the classification is not found
            std::cout<<"failure"<<std::endl;
            return;
        }
    }
    //if the last node is not terminal, it means the classification does not exist
    if (start->terminal == false) {
        std::cout<<"failure"<<std::endl;
        return;
    }
    start->terminal = false; //change the current node to not terminal because classification is getting erased

    TrieNode* parent = path.back(); // Parent of the last node
    int place = indexfound.back();

    bool hasChildren = false;
    for (TrieNode* child : start->children) { //see if it has any children
        if (child != nullptr) {
            hasChildren = true;
            break;
        }
    }

    if (!hasChildren && !start->terminal) {
        delete parent->children[place]; //deleting the node
        parent->children[place] = nullptr; //setting the pointer to null

        //checking if the parent node has any other children
        bool parentHasChildren = false;
        for (TrieNode* sibling : parent->children) {
            if (sibling != nullptr) {
                parentHasChildren = true;
                break;
            }
        }

        // If the parent has no children, mark it as terminal
        if (!parentHasChildren) {
            parent->terminal = true;
        }
    }
    sizeTrie--; //decrement the trie size
    std::cout<<"success"<<std::endl;
}

void Trie::print(){ //DONE
    if (sizeTrie == 0) { //the trie is empty, nothing to print
        std::cout << "trie is empty" << std::endl;
        return;
    }
    std::string output;
    std::string path;

    printPrint(root, path, output); //calling the helper function

    if (!output.empty() && output.back() != '_') {
        output += "_";
    }

    std::cout<<output<<std::endl;
}

void Trie::empty(){ //DONE
    if (sizeTrie == 0) { //if the size is 0, then the trie is empty
        std::cout<<"empty 1"<<std::endl;
    } else { //if not, then it is not empty
        std::cout<<"empty 0"<<std::endl;
    }
}

void Trie::clear(){ //DONE
    if (root != nullptr){
        clean(root); //calling the helper function to clear
        root = new TrieNode();
        sizeTrie = 0;
    }
    std::cout<<"success"<<std::endl;
}

void Trie::size(){ //DONE
    std::cout<<"number of classifications is "<<sizeTrie<<std::endl;
}

bool Trie::exit(){ //DONE
    exitsign = false;
    return exitsign;
}
