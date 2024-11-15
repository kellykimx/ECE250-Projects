#include "hashTable.h"
#include <iostream>
#include <string>
#include <vector>

hash::hash(int N, int T){
    this->size=N;
    this->collision=T;
    exitsign = true;
    hashTable = nullptr;
    checksumOP = nullptr;
    payload = nullptr;
};

hash::~hash(){
    if (hashTable != nullptr) {
        delete[] hashTable;
        hashTable = nullptr;
    }
    if (checksumOP != nullptr) {
        delete[] checksumOP;
        checksumOP = nullptr;
    }
    if (payload != nullptr) {
        delete[] payload;
        payload = nullptr;
    }
}

int hash::calculateCheckSum(const std::string& charstring) {
    int calcCheckSum = 0; //the sum of string
    int result;
    int length = charstring.length();
    for (int i=0; i<500; i++) {
        if (i < length) {
            calcCheckSum += charstring[i];
        } else { //if the length is bigger, then just add 0
            calcCheckSum += 0;
        }
    }
    result = calcCheckSum%256;
    return result;
}

//COMMANDS
void hash::newTable(int N, int T) { //DONE
    size=N;
    collision=T;
    //for open addressing
    if (collision == 0) {
        openaddressing = true; //this will be used to check whether we use open addressing or separate chaining for other functions
        hashTable = new int[size]; //where the ID will be stored
        payload = new std::string[size]; //where it will store the charstring
        for (int i=0; i<size; i++) {
            hashTable[i] = -100; //setting it to -100, indicating empty slot
            payload[i] = ""; //initializing payload as emoty string
        }
        checksumOP = new int[size];
        for (int i=0; i<size; i++) {
            checksumOP[i]=0;
        }
    //for separate chaining
    } else {
        openaddressing = false;
        //using a 2D vector, inner vector acts as a chain to store multiple keys that hash to the same index
        //allows dynamic resizing, vector automatically resizes
        chainTable = std::vector<std::vector<int>>(size);
        chainPayload = std::vector<std::vector<std::string>>(size);
        checksumSC = std::vector<std::vector<int>>(size);
    }
    //initializing all the checksum to 0
    std::cout<<"success"<<std::endl;
}

void hash::store(int ID, std::string charstring) { //DONE
    //removing the ! mark at the end
    if (!charstring.empty() && charstring.back() == '!') {
        charstring.pop_back();
    }
    //open addressing
    if (openaddressing) {
        int primaryHash = ID%size;
        if (hashTable[primaryHash] == -100) { // if the slot is empty, -100 indicating it is empty
            std::cout<<"success"<<std::endl; //the insertion was successful
            payload[primaryHash] = charstring; //storing charstring
            checksumOP[primaryHash]= calculateCheckSum(charstring); //storing the calculated checksum
            hashTable[primaryHash]=ID; //storing the id
        } else { //if the slot is already taken, double hashing
            bool found = false;
            int hashIncrement;
            int find=primaryHash;
            int secondaryHash = (ID/size)%size; //double hashing
            if ((secondaryHash%2) == 0) { //if it is even, add 1 to make it odd, so it goes through every slot
                hashIncrement = secondaryHash+1;
            }
            do {
                find=(find+hashIncrement)%size; //increment, finding an empty slot
                if (hashTable[find] == -100) {
                    hashTable[find] = ID;
                    std::cout<<"success"<<std::endl; //found an empty slot, insertion complete
                    found = true;
                    payload[find] = charstring;
                    checksumOP[find]= calculateCheckSum(charstring);
                    break;
                }
            } while (find != primaryHash); //until it returns to the intended slot

            if (!found) {
                std::cout<<"failure"<<std::endl; //no slots were empty
            }
        }
    //separate chaining
    } else {
        int position = ID % size;
        // Check for duplicates
        for (size_t i = 0; i < chainTable[position].size(); i++) {
            int duplicate = chainTable[position][i];
            if (duplicate == ID) { // Duplicate found
                std::cout << "failure" << std::endl;
                return;
            }
        }
        // if no duplicates were found, add it to the slot
        chainTable[position].push_back(ID); //adding ID to the back of the list
        chainPayload[position].push_back(charstring); //adding charstring to the back of the list
        checksumSC[position].push_back(calculateCheckSum(charstring)); //calculating checksum
        std::cout << "success" << std::endl;
    }
}

void hash::search(int ID) { //DONE
    //open addressing
    if (openaddressing) {
        int beginning = ID%size;
        int position = beginning;
        bool found = false;

        do {
            if (hashTable[position] == ID) { //checking if the ID matches
                std::cout<<"found " << ID << " in " << position << std::endl;
                found = true;
                break;
            }
            position = (position+1)%size; //moving onto the next position
        } while (beginning != position);

        if (!found) {
            std::cout<<"not found"<<std::endl;
        }

    } else { // separate chaining
        int position = ID % size;
        bool found = false;
        // Manually search through chainTable[position]
        for (size_t i = 0; i < chainTable[position].size(); ++i) {
            if (chainTable[position][i] == ID) { //going through the chain to check if the ID exists
                std::cout << "found " << ID << " in " << position << std::endl;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "not found" << std::endl;
        }
    }
}

void hash::deleteID(int ID) { //DONE
    if (openaddressing) { //open addressing
        bool found = false;
        int beginning = ID%size;
        for (int i=0; i<size; i++) {
            int position = (beginning+i) % size;

            if(hashTable[position]==ID) {
                std::cout<<"success"<<std::endl;
                hashTable[position]=-100; //deleted, resetting it to -100 which indicates empty slot
                payload[position].clear();
                checksumOP[position] = 0;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout<<"failure"<<std::endl;
        }
    //separate chaining
    } else {
        int position = ID % size;
        bool found = false;

        for (size_t i = 0; i < chainTable[position].size(); i++) { //going through the element in the chain
            if (chainTable[position][i] == ID) { // if ID found
                // Remove the ID and corresponding payload by index
                chainTable[position].erase(chainTable[position].begin() + i);
                chainPayload[position].erase(chainPayload[position].begin() + i);
                checksumSC[position].erase(checksumSC[position].begin() + i);
                std::cout << "success" << std::endl;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "failure" << std::endl;
        }
    }
}

void hash::corrupt(int ID, std::string newCharstring) {
    // Open Addressing
    if (openaddressing) {
        bool found = false;
        int primaryHash = ID % size;
        int find = primaryHash;
        int secondaryHash = (ID / size) % size;
        int hashIncrement = (secondaryHash % 2 == 0) ? secondaryHash + 1 : secondaryHash;

        do {
            if (hashTable[find] == ID) { // ID found, corrupt the existing charstring
                for (char &ch : payload[find]) {
                    ch = 0; // Set each character in the existing payload to zero
                }
                payload[find] = newCharstring; // Write the new charstring without changing the checksum
                std::cout << "success" << std::endl;
                return;
            }
            find = (find + hashIncrement) % size;
        } while (find != primaryHash);

        if (!found) {
            std::cout << "failure" << std::endl;
        }
    // Separate Chaining
    } else {
        int position = ID % size;
        bool found = false;

        for (size_t i = 0; i < chainTable[position].size(); ++i) { //looping through the IDs in the chain
            if (chainTable[position][i] == ID) {
                // ID found, corrupt the existing charstring
                for (char &ch : chainPayload[position][i]) {
                    ch = 0; // Set each character in the existing payload to zero
                }
                // Write the new charstring without changing the checksum
                chainPayload[position][i] = newCharstring;

                std::cout << "success" << std::endl;
                found = true;
                break;
            }
        }
        // If ID was not found, print failure
        if (!found) {
            std::cout << "failure" << std::endl;
        }
    }
}

void hash::validate(int ID) {
    //for open addressing
    if (openaddressing) {
        bool found = false;
        int primaryHash = ID%size;
        int hashIncrement;
        int find=primaryHash;
        int secondaryHash = (ID/size)%size;
        if ((secondaryHash%2) == 0) {
            hashIncrement = secondaryHash+1;
        }
        do {
            if (hashTable[find] == ID) { // Found the ID, validate checksum
                int computedChecksum = calculateCheckSum(payload[find]); //computing new checksum
                if (computedChecksum == checksumOP[find]) { //if the new and old is the same
                    std::cout << "valid" << std::endl; //checksum matches
                } else {
                    std::cout << "invalid" << std::endl; //if the checksum does not match
                }
                found=true;
                return;
            }
            find = (find + hashIncrement) % size;
        } while (find != primaryHash);

        if (!found) { // if ID is not found
            std::cout << "failure" << std::endl;
        }
    //for separate chaining
    } else {
        int position = ID % size;
        bool found = false;

        // Convert lists to vectors for index-based access
        std::vector<int> idVector(chainTable[position].begin(), chainTable[position].end());
        std::vector<std::string> payloadVector(chainPayload[position].begin(), chainPayload[position].end());

        // Search by index
        for (size_t i = 0; i < idVector.size(); ++i) {
            if (idVector[i] == ID) { // ID found, validate checksum
                int computedChecksum = calculateCheckSum(payloadVector[i]);
                if (computedChecksum == checksumSC[position][i]) {
                    std::cout << "valid" << std::endl; // if the checksum matches
                } else {
                    std::cout << "invalid" << std::endl; // if the checksum does not match
                }
                found = true;
                break;
            }
        }
        // If ID was not found, output failure
        if (!found) {
            std::cout << "failure" << std::endl;
        }
    }
}

void hash::print(int i) {
    if (chainTable[i].empty()) {
        std::cout << "chain is empty" << std::endl;
    } else {
        std::vector<int> sortedIDs = chainTable[i];
        // Manual bubble sort algorithm
        for (size_t j = 0; j < sortedIDs.size(); ++j) {
            for (size_t k = 0; k < sortedIDs.size() - j - 1; ++k) {
                if (sortedIDs[k] > sortedIDs[k + 1]) {
                    // Swap elements
                    int temp = sortedIDs[k];
                    sortedIDs[k] = sortedIDs[k + 1];
                    sortedIDs[k + 1] = temp;
                }
            }
        }
        for (int id : sortedIDs) {
            std::cout << id << " ";
        }
        std::cout << std::endl;
    }
}

bool hash::exit(){ //DONE
    exitsign = false;
    return exitsign;
}
