#ifndef ARRAYTYPE_HPP
#define ARRAYTYPE_HPP

#include "Type.hpp"

class ArrayType : public Type
{
public:
    ArrayType():Type(),baseType(nullptr),lowerBound(0){};
    ArrayType(unsigned size, Type* bt,int lb)
        :Type(size),baseType(bt),lowerBound(lb){};
    virtual ~ArrayType(){};

private:
    Type* baseType;
    int lowerBound;
};

#endif