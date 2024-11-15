#ifndef PROJECT1_CPU_H
#define PROJECT1_CPU_H

class CPU {
public:
    CPU(int n); //constructor
    ~CPU(); //destructor

    bool exitsign = true; //this is for exit
    void resize(int coreNum);
    int minTask(int coreNum);
    int maxTask(int coreNum);

    //COMMANDS
    void on(int n);
    void spawn(int p_id);
    void run(int c_id);
    void sleep(int c_id);
    void shutdown();
    void size (int c_id);
    void capacity(int c_id);
    bool exit();

private:
    int core; //number of cores
    int** queueTask; //current tasks in the queue, holding task ids for each core's queue
    int* queueSize; //holding how many tasks there are in each core's task queue
    int* coreCapacity;
};
#endif //PROJECT1_CPU_H
