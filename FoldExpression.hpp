#ifndef FOLD_EXPRESSION_HPP
#define FOLD_EXPRESSION_HPP

#include "Expression.hpp"

class FoldExpression : public Expression
{
public:
    FoldExpression() : Expression(), value(0){};
    int getValue(){return value;};
    void setValue(int v){value = v;};
private:
    int value;
};

#endif