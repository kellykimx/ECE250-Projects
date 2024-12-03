#include "graph.h"
#include "illegal.h"
#include <string>
#include <iostream>

int main() {
    std::string answer;
    std::string filename;
    std::string type;
    std::string sourceID, destinationID;
    std::string label;
    double weight;
    std::string name;
    std::string id;
    std::string id_1, id_2;
    std::string field_type, field_string;
    Graph* graph = new Graph();

    while (graph->exitsign) {
        std::cin>>answer;
        try {
            if (answer == "LOAD") {
                std::cin>>filename >> type;
                graph->load(filename, type);
            } else if (answer == "RELATIONSHIP") {
                std::cin>>sourceID;
                std::cin>>label;
                std::cin>>destinationID;
                std::cin>>weight;
                graph->relationship(sourceID, label, destinationID, weight);
            } else if (answer == "ENTITY") {
                std::cin>>id >> name >> type;
                graph->entity(id, name, type);
            } else if (answer == "PRINT") {
                std::cin>>id;
                graph->print(id);
            } else if (answer == "DELETE") {
                std::cin>>id;
                graph->deleteID(id);
            } else if (answer == "PATH") {
                std::cin>>id_1>>id_2;
                graph->path(id_1, id_2);
            } else if (answer == "HIGHEST") {
                graph->highest();
            } else if (answer == "FINDALL") {
                std::cin>>field_type;
                std::cin>>field_string;
                graph->findall(field_type, field_string);
            } else if (answer == "EXIT") {
                graph->exit();
            }
        } catch (illegal_exception& e) {
            std::cout<<e.what()<<std::endl; //print illegal argument
        }
    }
    delete graph;
    return 0;
}

