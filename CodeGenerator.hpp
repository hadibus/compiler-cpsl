#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <memory>
#include <string>

#include "SymbolTable.hpp"
#include "LvalExpression.hpp"

class CodeGenerator
{
public:

    CodeGenerator() : st(), expressions(), tempStrList(), tempExprIdxs()
    {
        st.initialize();
        printHeader();
    };

    ~CodeGenerator()
    {
        printFooter();
    }

    void assertIntOrChar(int);
    void appendStrList(char*);
    void makeVars(int, std::string reg = "$gp");
    int getLval(std::string);
    int assignExprToLval(int,int);
    void clearExpressions();
    int charLiteral(char);
    int charCast(int);
    int intLiteral(int);
    int intCast(int);
    int stringLiteral(char*);
    int getLint(char*);
    int loadReg(LvalExpression*);
    int binOp(int, int, int (CodeGenerator::*)(int, int), bool matchTypes = true);
    int binOpAdd(int, int);
    int binOpAnd(int, int);
    int binOpDiv(int, int);
    int binOpEq(int, int);
    int binOpGteq(int, int);
    int binOpGt(int, int);
    int binOpLteq(int, int);
    int binOpLt(int, int);
    int binOpMod(int, int);
    int binOpMult(int, int);
    int binOpNeq(int, int);
    int binOpOr(int, int);
    int binOpSub(int, int);

    int storeType(char*, int);

    int lookupType(char*);

    void printHeader();
    void printFooter();

    void writeExpression(int);
    void readToLval(int);

    void doStop();
    void storeConst(char*, int);
    

    int unOpNeg(int);
    int unOpNot(int);
    int unOpDecr(int);
    int unOpIncr(int);

private:
    SymbolTable st;
    std::vector<Expression*> expressions;
    std::vector<std::string> tempStrList;
    std::vector<int> tempExprIdxs;

};



#endif