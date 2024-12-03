#ifndef PROJECT4_GRAPH_H
#define PROJECT4_GRAPH_H

#include "graph_edge.h"
#include "graph_node.h"
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <iostream>


class Graph{
public:
    Graph();
    ~Graph();

    bool exitsign = true;//for exit function
    bool printSuccess; //load function will be calling entity and relationship function, to only print out "success" once
    void invalid(const std::string& input);

    //COMMANDS
    void load(const std::string& filename, const std::string type);
    void relationship (const std::string& sourceId, const std::string& label, const std::string& destinationId, double weight);
    void entity(const std::string id, const std::string name, const std::string type);
    void print(const std::string id);
    void deleteID(const std::string id);
    void path(const std::string id_1, const std::string id_2);
    void highest();
    void findall(const std::string field_type, const std::string field_string);
    bool exit();

private:
    std::vector<graphNode*> nodes;
    std::vector<graphEdge*> edges;
};
#endif //PROJECT4_GRAPH_H
