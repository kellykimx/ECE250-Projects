#include "hashTable.h"
#include <iostream>
#include <string>

int main() {
    std::string answer;
    int N=1;
    int T=0;
    int ID;
    int position; //input for print function
    std::string charstring;
    hash* hashTable = new hash(N, T);

    while (hashTable->exitsign) {
        std::cin >> answer;
        if (answer == "NEW") {
            std::cin>>N>>T;
            hashTable->newTable(N,T);
        } else if (answer == "STORE") {
            std::cin>>ID>>charstring;
            hashTable->store(ID, charstring);
        } else if (answer == "SEARCH") {
            std::cin>>ID;
            hashTable->search(ID);
        } else if (answer == "DELETE") {
            std::cin>>ID;
            hashTable->deleteID(ID);
        } else if (answer == "CORRUPT") {
            std::cin>>ID>>charstring;
            hashTable->corrupt(ID, charstring);
        } else if (answer == "VALIDATE") {
            std::cin>>ID;
            hashTable->validate(ID);
        } else if (answer == "PRINT") {
            std::cin>>position;
            hashTable->print(position);
        } else if (answer == "EXIT") {
            hashTable->exit();
        }
    }
    delete hashTable;
    return 0;
}
