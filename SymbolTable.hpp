#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "Frame.hpp"

class SymbolTable
{ 
public:
    SymbolTable(): stack(), globalFrame(), primitiveTypes(), stringList(), ineffableTypes(){};
    void initialize();
    Constant lookupConst(std::string);
    Variable lookupVar(std::string);
    int lookupType(std::string);
    Type * getType(std::string);
    void storeType(std::string, Type*);
    void storeConst(std::string, Type*, int);
    void storeVar(std::string, Type*, std::string);
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
    void changeFrameOffsetBy(int);
private:
    std::vector<Frame> stack;
    Frame globalFrame;
    std::vector<Type*> primitiveTypes;
    std::vector<std::string> stringList;
    std::vector<Type*> ineffableTypes;
};
#endif