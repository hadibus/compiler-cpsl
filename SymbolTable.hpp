#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <map>
#include <memory>
#include <vector>

#include "Constant.hpp"
#include "Variable.hpp"
#include "Function.hpp"

struct SymbolTableLayer
{
    SymbolTableLayer(): constants(), variables(), types(){};
    std::map<std::string, Constant> constants;
    std::map<std::string, Variable> variables;
    std::map<std::string, Type *> types;
};

class SymbolTable
{ 
public:
    SymbolTable(): declTypes(), stack(), primitiveTypes(), ineffableTypes(), stringList(), regPool(), frameOffsets(), functions(), inFunction(false), offset(0){};
    void initialize();
    Constant lookupConst(std::string);
    Variable lookupVar(std::string);
    int lookupType(std::string);
    Type * getType(std::string);
    void storeType(std::string, Type*);
    void storeConst(std::string, Type*, int);
    void storeVar(std::string, Type*, std::string, bool onStack = false);
    void storeVarStack(std::string, Type*);
    int storeStringLiteral(std::string);
    void checkForIdDefined(std::string);
    void enterScope();
    void leaveScope();
    std::shared_ptr<std::string> requestRegister();
    Type* getPrimitiveType(std::string);
    Type* getPrimitiveType(int);
    int addIneffableType(Type*);
    Type* getIneffableType(int);
    std::vector<std::string> getStringList();
    void changeVarOffset(std::string, int);
    void pushFrameOffset();
    void popFrameOffset();
    void changeFrameOffsetBy(int);
    void setInFunction(bool in){inFunction = in;}
private:
    std::vector<Type *> declTypes;
    std::vector<SymbolTableLayer> stack;
    std::vector<Type*> primitiveTypes;
    std::vector<Type*> ineffableTypes;
    std::vector<std::string> stringList;
    std::vector<std::shared_ptr<std::string>> regPool;
    std::vector<int> frameOffsets;
    std::map<std::string, Function> functions;
    bool inFunction;
    unsigned offset;
};
#endif