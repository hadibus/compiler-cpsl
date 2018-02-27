#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <map>
#include <memory>
#include <vector>

#include "Constant.hpp"
#include "Variable.hpp"

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
    SymbolTable(): toWriteList(), stack(), primativeTypes(), offset(0U), regPool(0U){};
    void initialize();
    Constant lookupConst(std::string);
    Variable lookupVar(std::string);
    Type * lookupType(std::string);
    void storeType(std::string, Type*);
    void storeConst(std::string, Type*, int);
    void storeVar(std::string, Type*, int, std::string);
    int storeStringLiteral(std::string);
    void checkForIdDefined(std::string);
    void enterScope();
    void leaveScope();
    Type* getPrimativeType(std::string);
    std::vector<std::string> getStringList();
private:
    std::vector<Type> toWriteList;
    std::vector<SymbolTableLayer> stack;
    std::vector<Type> primativeTypes;
    std::vector<std::string> stringList;
    unsigned offset;
    unsigned regPool;
};
#endif