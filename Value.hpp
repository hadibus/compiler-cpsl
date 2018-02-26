#ifndef VALUE_HPP
#define VALUE_HPP

#include "Type.hpp"

struct Value
{
    Value() : type(nullptr), value(0){};
    Type * type;
    int value;
};

#endif