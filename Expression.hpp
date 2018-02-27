#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Type.hpp"

class Expression
{
public:
    Expression():type(nullptr){};
    virtual ~Expression(){};
    Type * getType(){return type;};
    void setType(Type *t){type = t;};
private:
    Type * type;
};

#endif