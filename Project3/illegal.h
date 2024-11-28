#ifndef PROJECT3_ILLEGAL_H
#define PROJECT3_ILLEGAL_H

#include <exception>
#include <string>
#include <iostream>

class illegal_exception : public std::exception {
public:
    const char* what() {
        return "illegal argument";
    }
};

#endif //PROJECT3_ILLEGAL_H
