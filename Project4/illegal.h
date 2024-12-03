#ifndef PROJECT4_ILLEGAL_H
#define PROJECT4_ILLEGAL_H

#include <exception>
#include <string>
#include <iostream>

class illegal_exception : public std::exception {
public:
    const char* what() {
        return "illegal argument";
    }
};

#endif //PROJECT4_ILLEGAL_H
