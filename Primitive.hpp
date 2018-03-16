#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "Type.hpp"

class Primitive : public Type
{
public:
    Primitive():Type(4U){};
    Primitive(unsigned size):Type(size){};
    virtual ~Primitive(){};
};

#endif