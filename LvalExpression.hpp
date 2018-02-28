#ifndef LVAL_EXPRESSION_HPP
#define LVAL_EXPRESSION_HPP

#include "RegisterExpression.hpp"

class LvalExpression : public RegisterExpression 
{
public:
    LvalExpression() : RegisterExpression(), offset(0U){};
    void setOffset(int o){offset = o;};
    unsigned getOffset(){return offset;};
private:
    unsigned offset;
};

#endif