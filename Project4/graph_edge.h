#ifndef PROJECT4_GRAPH_EDGE_H
#define PROJECT4_GRAPH_EDGE_H

#include <string>
#include <iostream>

class graphEdge{
public:
    graphEdge(const std::string& sourceId, const std::string& destinationId, const std::string& label, double weight);
    //No need for a destructor, nothing to clean up
    std::string sourceId;
    std::string destinationId;
    std::string label;
    double weight;
};

#endif //PROJECT4_GRAPH_EDGE_H
