#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include "Type.hpp"

struct Constant
{
    Constant() : type(nullptr), value(0){};
    Type * type;
    int value;
};

#endif