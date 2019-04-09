#ifndef BOOLEANTYPE_HPP
#define BOOLEANTYPE_HPP

#include "Primitive.hpp"

class BooleanType : public Primitive
{
public:
    BooleanType():Primitive(4U){};
    virtual ~BooleanType(){};
};

#endif