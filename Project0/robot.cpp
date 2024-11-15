#include "environment.h"
#include <iostream>
#include <cmath>

Board::Board(int n, int m) {
    this->nRows=n;
    this->mColumns = m;
    potential = nullptr; // Initialize to nullptr
    item = nullptr;
    kConst=1;
}
Board::~Board(){
    clean();
}

void Board::clean(){
    if (potential) {
        for (int i = 0; i < nRows; i++) {
            delete[] potential[i];
        }
        delete[] potential;
        potential = nullptr;
    }

    if (item) {
        for (int i = 0; i < nRows; i++) {
            delete[] item[i];
        }
        delete[] item; //deleting goal, obstacle
        item = nullptr;
    }
}

//CALCULATING THE POTENTIAL
double Board::calculatePotential(int x, int y, int xG, int yG, char t) {
    double value;
    if((x==xG) && (y==yG)) {
        value = 0.0; //if denominator is 0, return 0
    } else {
        double distance = sqrt((pow(x- xG, 2)) + (pow(y - yG, 2)));
        value = kConst/distance;
    }

    if (t == 'G'){ //if goal, return negative value
        return -value;
    } else if (t =='O') { //if obstacle, return positive value
        return value;
    } else { //if nothing, return 0
        return 0.0;
    }
}

void Board::potentials(int x, int y, char t) {
    for (int xG=0; xG<nRows; xG++) { //going through each point and changing the potential value
        for (int yG=0; yG<mColumns; yG++) {
            if (xG >= 0 && xG < nRows && yG >=0 && yG < mColumns) {
                potential[xG][yG] += calculatePotential(x, y, xG, yG, t);
            }
        }
    }
}

void Board::calculateAll() {//calculating the whole grid with one function (it links to other calculating functions)
    for (int x = 0; x < nRows; x++) {
        for (int y = 0; y < mColumns; y++) {
            potential[x][y] = 0.0; // Resetting potential to 0
        }
    }

    for (int x=0; x<nRows; x++) {
        for (int y=0; y<mColumns; y++) {
            potentials(x, y, item[x][y]);
        }
    }
}
//COMMANDS
void Board::create(int n, int m) {
    if (n<=0 || m<=0) {
        std::cout<<"failure"<<std::endl;
        return;
    } else {
        clean();
        nRows = n;
        mColumns = m;
        //creating new board with n values of rows, m values of column
        potential = new double *[n]; //it is currently a 1D array; storing n pointers
        item = new char *[n];

        for (int i = 0; i < nRows; i++) {
            potential[i] = new double[m]; //this is to ensure that each element is pointing to an array with m elements
            item[i] = new char[m];
        }
        for (int i = 0; i < nRows; i++) {
            for (int k = 0; k < mColumns; k++) {
                potential[i][k] = 0.0; //initializing the potential value to zero
                item[i][k] = 'N'; //N stands for nothing, initializing the "item" at each point to nothing
            }
        }
        std::cout << "success" << std::endl;
    }
}

void Board::point(char t, int x, int y) {
    if ((x >= nRows || y >= mColumns) || (x < 0 || y < 0)) { //if x and y is out of the board
        std::cout<<"failure"<<std::endl;
    } else {
        item[x][y] = t;
        calculateAll();
        std::cout<<"success"<<std::endl;
    }
}

void Board::move(int x, int y) {
    if ((x >= 0 && x < nRows) && (y >= 0 && y < mColumns)) {
        std::cout<<potential[x][y] << " " << potential[x][y]<<std::endl;
    } else {
        std::cout<<"failure"<<std::endl;
    }
}

void Board::clear(){
    if (potential != nullptr) {
        for (int i = 0; i<nRows; i++) {
            for (int k = 0; k<mColumns; k++) {
                potential[i][k] = 0.0; //clearing all potential values as well as the item in each point
                item[i][k] = 'N';
            }
        }
        std::cout<<"success"<<std::endl;
    } else {
        std::cout<<"failure"<<std::endl;
    }
}

void Board::update(double k) {
    if (k <= 0) { //if k is negative or zero, it is a fail
        std::cout << "failure" << std::endl;
    } else {
        kConst = k; //if not, kConst is changed to a new k
        //call the function
        std::cout << "success" << std::endl;
        calculateAll();
    }
}

bool Board::exit() { //if this function is called then exitsign changes to true and breaks out of the loop
    exitsign = true;
    return exitsign;
}
