#include <iostream>
#include "environment.h"
#include <string>

int main() {
    std::string answer;
    int n=0, m=0; //nRows and mColumns
    int x, y; //x and y position
    double k=1.0; //k constant
    char item; //representing goal, obstacle, or nothing
    Board* board = nullptr;
    board = new Board(n, m);

    while (board->exit()) {
        std::cin>>answer;
        if (answer=="CREATE") {
            std::cin >> n;
            std::cin >> m;
            board->create(n,m);
        } else if (answer == "POINT") {
            std::cin >> item;
            std::cin >> x;
            std::cin >> y;
            board->point(item, x, y);
        } else if (answer == "MOVE") {
            std::cin>>x;
            std::cin>>y;
            board->move(x, y);
        } else if (answer == "CLEAR") {
            board->clear();
        } else if (answer == "UPDATE") {
            std::cin>>k;
            board->update(k);
        } else if (answer == "EXIT") {
            board->exit();
            break;
        }
    }
    delete board;
    return 0;
}
