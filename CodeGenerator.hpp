#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <memory>
#include <string>

#include "SymbolTable.hpp"
#include "LvalExpression.hpp"

class CodeGenerator
{
public:

    CodeGenerator() : st(), expressions(), tempStrList(), tempExprIdxs(),
        endifNumber(0), endifNumberStack(), forStack(), forAscendStack(),
        stringIdTemp()
    {
        st.initialize();
        printHeader();
        tempStrList.emplace_back();
    };

    ~CodeGenerator()
    {
        printFooter();
    }

    void assertIntOrChar(int);
    int appendStrList(int,char*);
    int makeStrList(char*);
    void makeVars(int,int, std::string reg = "$gp");
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

    int makeRecordVars(int,int);
    int mergeRecords(int, int);
    void addTempStrLayer();
    void removeTempStrLayer();

    int lookupType(char*);

    void printHeader();
    void printFooter();

    int preWhile();
    void startWhile(int,int);
    void endWhile(int);

    int startIf(int,bool);
    void endIf();
    void doElse(int);

    int startRepeat();
    void endRepeat(int,int);

    int startFor(char*,int);
    int compareFor(int,int,bool);
    void endFor(int);

    int writeExpression(int);
    void readToLval(int);

    void doStop();
    void storeConst(char*, int);
    
    int unOp(int,int (CodeGenerator::*)(int));
    int unOpNeg(int);
    int unOpNot(int);
    int unOpDecr(int);
    int unOpIncr(int);


    int getLvalArr(int,int);
    int getLvalRec(int,char*);

    int buildArray(int,int,int);

private:
    SymbolTable st;
    std::vector<Expression*> expressions;
    std::vector<std::vector<std::string>> tempStrList;
    std::vector<int> tempExprIdxs;
    int endifNumber;
    std::vector<int> endifNumberStack;
    std::vector<int> forStack;
    std::vector<bool> forAscendStack;
    std::string stringIdTemp;
    std::vector<bool> forNewVar;

};



#endif