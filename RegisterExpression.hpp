#ifndef REGISTER_EXPRESSION_HPP
#define REGISTER_EXPRESSION_HPP

#include <memory>
#include "Expression.hpp"

class RegisterExpression : public Expression
{
public:
    RegisterExpression() : Expression(), reg(){};
    void setRegister(const std::shared_ptr<std::string>& r){reg = r;};
    void releaseRegister(){reg.reset();};
    std::shared_ptr<std::string> getRegister(){return reg;};
private:
    std::shared_ptr<std::string> reg;
};
#endif