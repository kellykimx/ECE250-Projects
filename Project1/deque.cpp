#include "cpu.h"
#include <iostream>

CPU::CPU(int n){
    this->core=n;
    queueTask = nullptr;
    queueSize = nullptr;
    coreCapacity = nullptr;
}

CPU::~CPU() {
    if (queueTask != nullptr) {
        for (int i = 0; i < core; i++) {
            delete[] queueTask[i];
        }
        delete[] queueTask;
        queueTask = nullptr;
    }
    if (queueSize != nullptr) {
        delete[] queueSize;
        queueSize = nullptr;
    }
    if(coreCapacity != nullptr) {
        delete[] coreCapacity;
        coreCapacity = nullptr;
    }
}

//resizing capacity  (IS CAPACITY ALWAYS 4?) capacity is how much task each core can take
/* need to take consideration of:
Whenever size reaches capacity, the capacity must be doubled and the array resized, with all elements being copied to the new array.
Whenever size reaches ¼ of capacity, the capacity must be halved and the array resized, with all elements being copied to the new array.
The minimum capacity is 2.
*/
void CPU::resize(int coreNum) {
    int currentCapacity = coreCapacity[coreNum];
    int currentSize = queueSize[coreNum];

    if (currentSize == 0) {
        currentCapacity = 2;
        int *newArray = new int[currentCapacity];
        delete[] queueTask[coreNum];
        queueTask[coreNum] = newArray;
        coreCapacity[coreNum] = currentCapacity; //change capacity
    } else {
        //if size reaches capacity
        if (currentCapacity == currentSize) {
            currentCapacity = currentCapacity * 2; //double the capacity
            int *newArray = new int[currentCapacity]; //create a new array

            for (int i = 0; i < currentSize; i++) {
                newArray[i] = queueTask[coreNum][i]; //move the tasks into the new array
            }
            delete[] queueTask[coreNum]; //delete old
            queueTask[coreNum] = newArray; //add new
            coreCapacity[coreNum] = currentCapacity; //change capacity

            // if size reaches 1/4 of the capacity, capacity must be halved
        } else if (currentSize <= (currentCapacity / 4) && currentCapacity > 2) {
            currentCapacity = currentCapacity / 2; //half the capacity
            if (currentCapacity < 2) {
                currentCapacity = 2;
            } else {
                currentCapacity = currentCapacity;
            }
            int *newArray = new int[currentCapacity]; //create a new array

            for (int i = 0; i < currentSize; i++) { //add the task into new array
                newArray[i] = queueTask[coreNum][i];
            }
            delete[] queueTask[coreNum]; //delete old
            queueTask[coreNum] = newArray; //add new
            coreCapacity[coreNum] = currentCapacity; //change capacity value
        } else {
            return;
        }
    }
}
// this function is used for sleep function
int CPU::minTask(int coreNum) { // this function is taking the c_id, the exception
    int minTaskCore = -1;
    int taskNum = 100000;
    for (int i=0; i<core; i++) {
        if ((queueSize[i] < taskNum) && (i != coreNum)) { //excluding c_id that will be shutting down
            taskNum = queueSize[i]; //resetting the taskNum to check other core values
            minTaskCore = i; //core number is changing
        }
    }
    return minTaskCore; //at the end, return the core number with the lowest task
}

//this function is used for run function
int CPU::maxTask(int coreNum) {
    int maxTaskCore=0;
    int taskNum=0;
    for (int i=0; i<core; i++) {
        if((queueSize[i]>taskNum) && (i != coreNum)) {
            taskNum = queueSize[i];
            maxTaskCore = i;
        }
    }
    return maxTaskCore;
}

//COMMANDS
void CPU::on(int n) { //DONE
    if (queueSize != nullptr) { //checking if CPU was already created
        std::cout<<"failure"<<std::endl;
    } else {
        core = n;
        queueSize = new int[core]; // size of each core
        queueTask = new int*[core]; //points to the task for core
        coreCapacity = new int[core];
        for(int i=0; i<core; i++){
            queueTask[i]=new int[4];
            queueSize[i]=0; //to make sure there are no tasks assigned to queue
            coreCapacity[i]=4;
        }
        std::cout<<"success"<<std::endl;
    }
}

void CPU::spawn(int p_id){ //DONE, generating a new task and adding that to the core with the lowest task
    if(p_id <= 0) {   //checking if it is negative
        std::cout<<"failure"<<std::endl;
    } else {
        int selected=0; //this is to keep track of what core was selected
        int lowestTask = queueSize[0];
        for(int i=0; i<core; i++) {
            if (queueSize[i] < lowestTask) { //if the current core's size of the task is smaller than the lowestTask
                lowestTask = queueSize[i];
                selected = i;
            } else if (queueSize[i]==lowestTask) { //if the task size is equal
                if (i<selected) {  //select the lowest index core
                    selected = i;
                } else {
                    selected = selected;
                }
            }
        }
        resize(selected);
        queueTask[selected][queueSize[selected]] = p_id; //assigning the task p_id into the core
        queueSize[selected]=queueSize[selected]+1;
        resize(selected);
        std::cout<<"core " << selected << " assigned task " << p_id << std::endl;
    }
}
void CPU::run(int c_id) {
    if (c_id < 0 || c_id > (core - 1)) { //if the queue is empty
        std::cout << "failure" << std::endl;
    } else if (queueSize[c_id] == 0) {  //if c_id is out of range
        std::cout << "core " << c_id << " has no tasks to run" << std::endl;
        if (queueSize[c_id] == 0) {
            int maxTaskNum = maxTask(c_id);
            if ((queueSize[maxTaskNum] > 0) && maxTaskNum != -1) {
                queueTask[c_id][queueSize[c_id]] = queueTask[maxTaskNum][queueSize[maxTaskNum] - 1];
                queueSize[c_id] = queueSize[c_id] + 1;
                queueSize[maxTaskNum] = queueSize[maxTaskNum] - 1;
                resize(maxTaskNum);
                resize(c_id);
            } else {
                return;
            }
        }
    } else {
        int currentTask = queueTask[c_id][0]; //the first task in the core c_id
        std::cout << "core " << c_id << " is running task " << currentTask << std::endl;

        for (int i = 0; i < queueSize[c_id] - 1; i++) {
            queueTask[c_id][i] = queueTask[c_id][i + 1]; //moving every task in the queue front
        }
        queueSize[c_id] = queueSize[c_id] - 1; //reducing the task size
        if (queueSize[c_id] == 0) {
            int maxTaskNum = maxTask(c_id);
            if ((queueSize[maxTaskNum] > 0) && maxTaskNum != -1) {
                queueTask[c_id][queueSize[c_id]] = queueTask[maxTaskNum][queueSize[maxTaskNum] - 1];
                queueSize[c_id] = queueSize[c_id] + 1;
                queueSize[maxTaskNum] = queueSize[maxTaskNum] - 1;
                resize(maxTaskNum);
                resize(c_id);
            } else {
                return;
            }
        }
    }
}
void CPU::sleep(int c_id){ //DONE
    if (c_id < 0 || c_id>(core-1)) {
        std::cout<<"failure"<<std::endl;
    } else if (queueSize[c_id] == 0) {
        std::cout<<"core " << c_id << " has no tasks to assign" << std::endl;
    } else {
        //reassigning tasks in c_id to cores with the least amount of tasks
        int tasks = queueSize[c_id];
        for (int i=0; i<tasks; i++) {
            int lowestTaskCore = minTask(c_id);
            resize(lowestTaskCore);

            queueTask[lowestTaskCore][queueSize[lowestTaskCore]] = queueTask[c_id][queueSize[c_id]-1]; //adding the task to the queue
            queueSize[lowestTaskCore]=queueSize[lowestTaskCore]+1; //increasing the size
            std::cout<<"task "<<queueTask[c_id][queueSize[c_id]-1]<< " assigned to core " << lowestTaskCore << " ";
            queueSize[c_id]--;
        }
        std::cout<<std::endl;
        queueSize[c_id]=0;
        resize(c_id);
    }
}

void CPU::shutdown() { //DONE
    //pops all remaining tasks off of all remaining queues
    int emptyCore=0;
    for (int i=0; i<core; i++) {  //checking there is no tasks in any cores
        if (queueSize[i] == 0) {
            emptyCore++;
        }
    }
    if (emptyCore==core) {    //if the counter is equal to the number of cores, all cores are empty
        std::cout<<"no tasks to remove"<<std::endl;
    } else {
        for (int i = 0; i < core; i++) {
            for (int k = 0; k < queueSize[i]; k++) {  //for each core's task, delete it
                std::cout << "deleting task " << queueTask[i][k] << " from core " << i << " ";
            }
            queueSize[i] = 0; //deleting all tasks in core i
            resize(i);
        }
        std::cout<<std::endl;
    }
}

void CPU::size(int c_id) { //DONE, printing the number of tasks in core
    if (c_id<0 || c_id>(core-1)) { //checking the range
        std::cout<<"failure"<<std::endl;
    } else {
        std::cout<<"size is " << queueSize[c_id] << std::endl; //printing out the size of core c_id
    }
}

/* need to take consideration of:
Whenever size reaches capacity, the capacity must be doubled and the array resized, with all elements being copied to the new array.
Whenever size reaches ¼ of capacity, the capacity must be halved and the array resized, with all elements being copied to the new array.
The minimum capacity is 2.
*/
void CPU::capacity(int c_id) { //DONE
    if (c_id<0 || c_id>(core-1)) { //checking the range
        std::cout<<"failure"<<std::endl;
    } else {
        std::cout<<"capacity is "<< coreCapacity[c_id]<<std::endl;
    }
}

bool CPU::exit() { //DONE
    exitsign = false;
    return exitsign;
}
