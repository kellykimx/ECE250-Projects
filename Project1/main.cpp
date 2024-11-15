#include "cpu.h"
#include <iostream>
#include <string>

int main() {
    std::string answer;
    int n=2;
    int p_id;
    int c_id;
    CPU* cpu =new CPU(n);

    while (cpu->exitsign) {
        std::cin >> answer;
        if (answer == "ON") {
            std::cin>>n;
            cpu->on(n);
        } else if (answer == "SPAWN") {
            std::cin>>p_id;
            cpu->spawn(p_id);
        } else if (answer == "RUN") {
            std::cin>>c_id;
            cpu->run(c_id);
        } else if (answer == "SLEEP") {
            std::cin>>c_id;
            cpu->sleep(c_id);
        } else if (answer == "SHUTDOWN") {
            cpu->shutdown();
        } else if (answer == "SIZE") {
            std::cin>>c_id;
            cpu->size(c_id);
        } else if (answer == "CAPACITY") {
            std::cin>>c_id;
            cpu->capacity(c_id);
        } else if (answer == "EXIT") {
            cpu->exit();
        }
    }
    delete cpu;
    return 0;
}
