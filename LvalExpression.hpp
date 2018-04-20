#ifndef LVAL_EXPRESSION_HPP
#define LVAL_EXPRESSION_HPP

#include "RegisterExpression.hpp"

class LvalExpression : public RegisterExpression 
{
public:
    LvalExpression() : RegisterExpression(), offset(0){};
    void setOffset(int o){offset = o;};
    int getOffset(){return offset;};
private:
    int offset;
};

#endif