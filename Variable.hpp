#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>

#include "Type.hpp"

struct Variable
{
    Variable() : type(nullptr), offset(0), reg(){};
    Type * type;
    int offset;
    std::string reg;
};

#endif