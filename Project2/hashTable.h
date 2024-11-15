#ifndef PROJECT2_HASHTABLE_H
#define PROJECT2_HASHTABLE_H
#include <string>
#include <vector>

class hash{
public:
    hash(int N, int T);
    ~hash();

    bool exitsign = true;//for exit function

    int calculateCheckSum(const std::string& charstring);

    //COMMANDS
    void newTable(int N, int T);
    void store(int ID, std::string charstring);
    void search(int ID);
    void deleteID(int ID);
    void corrupt(int ID, std::string newCharstring);
    void validate(int ID);
    void print(int i);
    bool exit();

private:
    int size;
    int collision;
    bool openaddressing = true; //checking if it is open addressing or separate chaining
    int* hashTable;
    int* checksumOP;
    std::string* payload;
    std::vector<std::vector<int>> chainTable;
    std::vector<std::vector<std::string>> chainPayload;
    std::vector<std::vector<int>> checksumSC;


};

#endif //PROJECT2_HASHTABLE_H
