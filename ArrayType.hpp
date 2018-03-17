#ifndef ARRAYTYPE_HPP
#define ARRAYTYPE_HPP

#include "Type.hpp"

class ArrayType : public Type
{
public:
    ArrayType():Type(),baseType(nullptr),lowerBound(0){};
    ArrayType(unsigned size, Type* bt,int lb)
        :Type(size),baseType(bt),lowerBound(lb){};
    int getLowerBound(){return lowerBound;};
    int getBaseTypeSize(){return baseType->size();};
    Type* getBaseType(){return baseType;};
    //don't include current level's size
    int getBaseSizeRecusive(){return baseType->getSizeRecursive();}
    int getSizeRecursive()
    {
        return size() * baseType->getSizeRecursive();
    };
    virtual ~ArrayType(){};

private:
    Type* baseType;
    int lowerBound;
};

#endif