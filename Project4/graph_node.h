#ifndef PROJECT4_GRAPH_NODE_H
#define PROJECT4_GRAPH_NODE_H

#include <string>
#include <iostream>

class graphNode{
public:
    graphNode(const std::string& id, const std::string& name, const std::string& type);
    //No need for a destructor, nothing to clean up
    std::string id;
    std::string name;
    std::string type;
};

#endif //PROJECT4_GRAPH_NODE_H
