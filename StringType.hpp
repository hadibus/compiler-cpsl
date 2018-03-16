#ifndef STRINGTYPE_HPP
#define STRINGTYPE_HPP

#include "Primitive.hpp"

class StringType : public Primitive
{
public:
    StringType():Primitive(4U){};
    virtual ~StringType(){};
};

#endif