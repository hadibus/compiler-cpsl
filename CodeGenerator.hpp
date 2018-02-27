#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <string>

#include "SymbolTable.hpp"
#include "Value.hpp"

class CodeGenerator
{
public:

    CodeGenerator() : st()
    {
        st.initialize();
        printHeader();
    };

    ~CodeGenerator()
    {
        printFooter();
    }

    void assertIntOrChar(Value);

    
    Value charLiteral(char);
    Value charCast(Value);
    Value intLiteral(int);
    Value intCast(Value);
    int stringLiteral(char*);
    Value getLValue(char*);
    Value binOpAdd(Value, Value);
    Value binOpAnd(Value, Value);
    Value binOpDiv(Value, Value);
    Value binOpEq(Value, Value);
    Value binOpGteq(Value, Value);
    Value binOpGt(Value, Value);
    Value binOpLteq(Value, Value);
    Value binOpLt(Value, Value);
    Value binOpMod(Value, Value);
    Value binOpMult(Value, Value);
    Value binOpNeq(Value, Value);
    Value binOpOr(Value, Value);

    Value binOpSub(Value, Value);

    void printHeader();
    void printFooter();

    void writeExpression();

    void doStop();

    

    int unOpNeg(int);
    int unOpNot(int);
    int unOpDecr(int);
    int unOpIncr(int);

private:
    SymbolTable st;
    std::vector<Value> values;

};



#endif