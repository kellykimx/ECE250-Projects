#include "graph.h"
#include "graph_node.h"
#include "graph_edge.h"
#include "illegal.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

graphNode::graphNode(const std::string& id, const std::string& name, const std::string& type) { //constructor for graph_node
    this->id=id;
    this->name=name;
    this->type=type;
}

graphEdge::graphEdge(const std::string& sourceId, const std::string& destinationId, const std::string& label, double weight) {
    this->sourceId = sourceId;
    this->destinationId = destinationId;
    this->label = label;
    this->weight = weight;
}

Graph::Graph(){ //constructor for graph
    //clearing nodes and edges, making sure they are empty
    nodes.clear();
    edges.clear();
    printSuccess = true;
}

Graph::~Graph(){ //destructor for graph
    while (!nodes.empty()) { //while the nodes are not empty
        delete nodes.back(); // delete the node at the back and
        nodes.pop_back();    // remove it from the vector
    }
    while (!edges.empty()) { //while the edges are not empty
        delete edges.back(); // delete the last edge
        edges.pop_back();    // remove it from the vector
    }
}

//HELPER FUNCTIONS
void Graph::invalid(const std::string& input) { //this is for illegal exceptions, checking its validity
    for (char c : input) {
        //checking if it has any other characters other than upper case letters, lower case letters, numerals
        if (!std::isupper(c) && !std::islower(c) && !std::isdigit(c)) {
            throw illegal_exception(); // "illegal argument"
        }
    }
}

//COMMANDS
void Graph::load(const std::string& filename, const std::string type){ //DONE
    std::ifstream file(filename);
    std::string line; //line stores each line of file

    printSuccess=false;

    //reading every line of code
    while(std::getline(file, line)) {
        std::stringstream stream(line);
        if (type == "entities") { //if the type is entities
            std::string id, name, nodeType;
            stream >> id >> name >> nodeType;
            entity(id, name, nodeType); //calling the entity function to insert/update
        } else if (type == "relationships") { //if the type is relationships
            std::string sourceId, label, destinationId;
            double weight;
            stream>>sourceId>>label>>destinationId>>weight;
            relationship(sourceId, label, destinationId, weight); //calling the relationship function to insert/update
        }
    }
    file.close();
    printSuccess = true;
    std::cout<<"success"<<std::endl;
}

void Graph::relationship (const std::string& sourceId, const std::string& label, const std::string& destinationId, double weight){ //DONE
    //variables to check if the vertices exist
    graphNode* sourceNode = nullptr;
    graphNode* destinationNode = nullptr;

    bool found1 = false; //marker to check if vertices exist or not
    bool found2 = false;

    //for loop to find the source and destination nodes
    for (size_t i=0; i<nodes.size(); i++) { //iterating through nodes to look for the vertices
        if (nodes[i] == nullptr) {
            continue;  // Skip invalid nodes
        }
        if(nodes[i]->id == sourceId){ //if they exist
            sourceNode =nodes[i]; //store it into the sourceNode
            found1 = true; //and mark found
        }
        if(nodes[i]->id == destinationId){
            destinationNode = nodes[i];
            found2 = true;
        }
        if (found1 && found2) {
            break;
        }
    }
    //if either vertices are not found, then output failure
    if ((found1 == false) || (found2 == false)) {
        std::cout<<"failure"<<std::endl;
        return;
    }

    //this is to update the label and weight if edge already exists between two vertices
    for (size_t i=0; i<edges.size(); i++) {
        graphEdge* edge = edges[i];
        if((edge->sourceId == sourceId && edge->destinationId == destinationId) ||
           (edge->sourceId == destinationId && edge->destinationId==sourceId)) {
            edge->label = label; //updating the label
            edge->weight = weight; //updating the weight
            if (printSuccess) {
                std::cout<<"success"<<std::endl;
            }
            return;
        }
    }
    //if no edge exists, then create a new edge
    graphEdge* newEdge = new graphEdge(sourceId, destinationId, label, weight);
    edges.push_back(newEdge); //add the new edge to the vector
    if (printSuccess) {
        std::cout<<"success"<<std::endl;
    }
}

void Graph::entity(const std::string id, const std::string name, const std::string type){ //DONE
    //this function always succeeds
    for (size_t i=0; i<nodes.size(); i++) {
        if (nodes[i]->id == id){ //if the given ID already exists, update its name and type
            nodes[i]->name=name; //updating its name
            nodes[i]->type=type; //updating its type
            if (printSuccess) {
                std::cout << "success" << std::endl;
            }
            return;
        }
    }
    //if the entity does not exist, create a new entity
    graphNode* newNode = new graphNode(id, name, type);
    nodes.push_back(newNode); //adding the new node to the vector
    if (printSuccess) {
        std::cout << "success" << std::endl;
    }
}

void Graph::print(const std::string id){ //DONE
    //illegal exceptions
    try {
        invalid(id);
    } catch (illegal_exception& e) {
        std::cout<<e.what()<<std::endl;
        return;
    }

    //find the node with the given id
    graphNode* findNode = nullptr;
    bool found = false;
    for (size_t i=0; i<nodes.size(); i++) {
        if(nodes[i]->id == id) {
            findNode = nodes[i];
            found = true;
            break;
        }
    }
    //if the vertex does not exist
    if(found == false) {
        std::cout<<"failure"<<std::endl;
        return;
    }

    std::vector<std::string> adjacent; //this stores the id of adjacent vertices
    for (size_t i=0; i<edges.size(); i++) { //going through all edges
        if (edges[i]->sourceId == id) { //checking if the sourceID matches with the given id
            adjacent.push_back(edges[i]->destinationId); //add the other end of the edge
        } else if (edges[i]->destinationId == id) {
            adjacent.push_back(edges[i]->sourceId); //add the other end of the edge
        }
    }

    //printing the result
    if (adjacent.empty()) {
        std::cout<<std::endl; //if there are no adjacent vertices, print a blank line
    } else {
        for (size_t i=0; i < adjacent.size(); i++) {
            std::cout<<adjacent[i]; //printing the vertex
            if (i < adjacent.size() -1) {
                std::cout<<" "; //spaces in between vertex IDs
            }
        }
        std::cout<<std::endl; //followed by a new line character
    }
}

void Graph::deleteID(const std::string id){  //delete the vertex with the given id and any edges containing it, DONE
    //illegal exceptions
    try {
        invalid(id);
    } catch (illegal_exception& e) {
        std::cout<<e.what()<<std::endl;
        return;
    }

    graphNode *deleteVertex = nullptr; //this is where the vertex with the given id will be stored
    size_t index = 0;
    bool found = false;

    //find the vertex with the given id
    for(size_t i=0; i<nodes.size(); i++) {
        if (nodes[i]->id == id) {
            deleteVertex = nodes[i];
            index = i;
            found = true;
            break;
        }
    }
    //if the vertex with the given id is not found, does not exist
    if(found == false) {
        std::cout<<"failure"<<std::endl;
        return;
    }

    delete deleteVertex;
    nodes.erase(nodes.begin()+index);

    //removing all edges connected to the vertex
    for (size_t i=0; i<edges.size();) {
        //check if the edge is connected to the vertex
        if (edges[i]->sourceId==id || edges[i]->destinationId == id) {
            delete edges[i];
            edges.erase(edges.begin()+i); //remove the edge from the edge vector
            //no need to increment i, since erase already shifts the elements to the left
        } else {
            //increment i only if the edge has not been removed
            i++;
        }
    }
    std::cout<<"success"<<std::endl;
}

void Graph::path(const std::string id_1, const std::string id_2) {
    try {
        invalid(id_1); //checking if id_1 is valid
        invalid(id_2); //checking if id_2 is valid
    } catch (illegal_exception& e) {
        std::cout<<e.what()<<std::endl; //"illegal arguments"
        return;
    }
    int start = -1; //index of the starting vertex
    int end = -1; //index of the ending vertex

    bool found1 = false; //marker to see if id_1 is in graph
    bool found2 = false; //marker to see if id_2 is in graph

    //check if both vertices exist in the graph
    for (size_t i=0; i<nodes.size(); i++) {
        if (nodes[i]->id == id_1) {
            start = i;
            found1 = true;
        }
        if (nodes[i]->id == id_2) {
            end = i;
            found2 = true;
        }
    }
    //any one of the vertices is not in graph
    if (start == -1 || end==-1) {
        std::cout<<"failure"<<std::endl;
        return;
    }

    //vectors to track highest weight and previous nodes
    std::vector<double> highestWeights(nodes.size(), -1); //highest weight to each node
    std::vector<int> previous(nodes.size(), -1); //previous node in the path for each node
    std::vector<bool> visited(nodes.size(), false); //to track if the node has been visited or not

    highestWeights[start]=0; //start with the weight 0

    std::vector<std::tuple<double, int>> priorityQueue; //storing their current weights
    priorityQueue.push_back(std::make_tuple(0, start));

    while(priorityQueue.empty()==false) {
        //first sort the priority queue by weight in descending order
        std::sort(priorityQueue.begin(), priorityQueue.end(), [](const std::tuple<double, int>& a, const std::tuple<double, int>& b) {
            return std::get<0>(a) > std::get<0> (b);
        });
        //getting the front, one with the highest weight
        std::tuple<double, int> current = priorityQueue.front();
        priorityQueue.erase(priorityQueue.begin()); //remove it from the queue

        double currentWeight = std::get<0>(current);
        int currentNode = std::get<1>(current);

        if (visited[currentNode]) {
            continue; //skip if the node has already been visited
        }
        visited[currentNode]=true;

        for (size_t i=0; i<edges.size(); i++) {
            int updateIndex = -1; //node that is directly connected to another node by an edge
            double edgeWeight = edges[i]->weight; //weight of the current edge

            //checking if the current node is the source of the edge
            if (edges[i]->sourceId == nodes[currentNode]->id) {
                for (size_t j=0; j<nodes.size(); j++) {
                    if (nodes[j]->id == edges[i]->destinationId) {
                        updateIndex = j;
                        break;
                    }
                }
                //checking if the current node is the destination of the edge
            } else if (edges[i]->destinationId == nodes[currentNode]->id) {
                for (size_t j = 0; j < nodes.size(); j++) {
                    if (nodes[j]->id == edges[i]->sourceId) {
                        updateIndex = j;
                        break;
                    }
                }
            }
            //if it does not exist or has been visited, skip
            if (updateIndex == -1 || visited[updateIndex]) {
                continue;
            }

            double newWeight = currentWeight + edgeWeight;
            if (newWeight > highestWeights[updateIndex]) {
                highestWeights[updateIndex] = newWeight; //update the highest weight
                previous[updateIndex]=currentNode; //update previous node
                priorityQueue.push_back(std::make_tuple(newWeight, updateIndex)); //add the update to the queue
            }
        }
    }
    //if there was no path that goes to the end node
    if (highestWeights[end] == -1) {
        std::cout<<"failure"<<std::endl;
        return;
    }
    //constructing the path from the end node to the start node
    std::vector<std::string> path;
    int currentposition = end;

    while (currentposition != -1) { //starting from the end
        path.push_back(nodes[currentposition]->id); //add current node's id to the path
        currentposition = previous[currentposition]; //move to the previous node
    }
    std::reverse(path.begin(), path.end());

    for (size_t i=0; i<path.size(); i++) {
        std::cout<<path[i]; //print each node in path
        if(i < path.size()-1) {
            std::cout<<" "; //this is to print a space in between nodes
        }
    }
    std::cout<<" "<<highestWeights[end]<<std::endl;
}

void Graph::highest() {
    if (edges.empty()) { //if the graph has zero edges
        std::cout << "failure" << std::endl;
        return;
    }

    double highestPathWeight = -1; // track the highest weight path
    std::string start, end; // track start and end nodes of the highest path

    for (size_t i = 0; i < nodes.size(); i++) {
        std::string startNode = nodes[i]->id;
        // initialize vectors for weights and visited nodes
        std::vector<double> highestWeights(nodes.size(), -1);
        std::vector<bool> visited(nodes.size(), false);
        highestWeights[i] = 0; // start with the current node

        std::vector<std::tuple<double, int>> priorityQueue;
        priorityQueue.push_back(std::make_tuple(0.0, static_cast<int>(i))); // push the starting node

        while (!priorityQueue.empty()) {
            // find the node with the highest weight in the queue
            size_t highestIndex = 0;
            for (size_t j = 1; j < priorityQueue.size(); j++) {
                if (std::get<0>(priorityQueue[j]) > std::get<0>(priorityQueue[highestIndex])) {
                    highestIndex = j;
                }
            }
            //taking the highest weight
            std::tuple<double, int> current = priorityQueue[highestIndex];
            priorityQueue.erase(priorityQueue.begin() + highestIndex);

            double currentWeight = std::get<0>(current);
            int currentNode = std::get<1>(current);

            if (visited[currentNode]) {
                continue;
            }
            visited[currentNode] = true;

            // edges connected to the current node
            for (size_t j = 0; j < edges.size(); j++) {
                int updateIndex = -1;
                double edgeWeight = edges[j]->weight;

                // check if the current node is the source of the edge
                if (edges[j]->sourceId == nodes[currentNode]->id) {
                    for (size_t k = 0; k < nodes.size(); k++) {
                        if (nodes[k]->id == edges[j]->destinationId) {
                            updateIndex = static_cast<int>(k);
                            break;
                        }
                    }
                }
                    // checking if the current node is the destination of the edge
                else if (edges[j]->destinationId == nodes[currentNode]->id) {
                    for (size_t k = 0; k < nodes.size(); k++) {
                        if (nodes[k]->id == edges[j]->sourceId) {
                            updateIndex = static_cast<int>(k);
                            break;
                        }
                    }
                }

                if (updateIndex == -1 || visited[updateIndex]) {
                    continue;
                }

                double newWeight = currentWeight + edgeWeight;
                if (newWeight > highestWeights[updateIndex]) { //if higher weight, update
                    highestWeights[updateIndex] = newWeight;
                    priorityQueue.push_back(std::make_tuple(newWeight, updateIndex));

                    // update the highest path details
                    if (newWeight > highestPathWeight) {
                        highestPathWeight = newWeight;
                        start = startNode;
                        end = nodes[updateIndex]->id;
                    }
                }
            }
        }
    }

    if (highestPathWeight == -1) {
        std::cout << "failure" << std::endl;
    } else {
        std::cout << start << " " << end << " " << highestPathWeight << std::endl;
    }
}

void Graph::findall(const std::string field_type, const std::string field_string) {
    std::vector<std::string> result; //storing the ids of nodes that match the description

    for (size_t i=0; i<nodes.size(); i++) { //iterating through all nodes
        if (field_type == "name" && nodes[i]->name == field_string){
            result.push_back(nodes[i]->id); //if it matches, add it to the result vector
        } else if (field_type == "type" && nodes[i]->type == field_string) {
            result.push_back(nodes[i]->id);
        }
    }
    //if no vertices of the given field_type exist
    if (result.empty()) {
        std::cout<<"failure"<<std::endl;
    } else {
        for (size_t i=0; i < result.size(); i++) {
            std::cout<<result[i]; //print the ids of each matching node
            if (i < result.size()-1) {
                std::cout<<" "; //add space between IDs
            }
        }
        std::cout << std::endl; //new line at the end
    }
}

bool Graph::exit(){ //DONE
    exitsign = false;
    return exitsign;
}
