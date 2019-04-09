#ifndef INTEGERTYPE_HPP
#define INTEGERTYPE_HPP

#include "Primitive.hpp"

class IntegerType : public Primitive
{
public:
    IntegerType():Primitive(4U){};
    virtual ~IntegerType(){};
};

#endif