#include <stdexcept>

#include "SymbolTable.hpp"

void SymbolTable::initialize()
{
    primativeTypes.emplace_back("integer");
    primativeTypes.emplace_back("char");
    primativeTypes.emplace_back("boolean");
    primativeTypes.emplace_back("string");

    enterScope();
    storeType("integer", &primativeTypes[0]);
    storeType("INTEGER", &primativeTypes[0]);
    storeType("char", &primativeTypes[1]);
    storeType("CHAR", &primativeTypes[1]);
    storeType("boolean", &primativeTypes[2]);
    storeType("BOOLEAN", &primativeTypes[2]);
    storeType("string", &primativeTypes[3]);
    storeType("STRING", &primativeTypes[3]);

    storeConst("true", &primativeTypes[2], 1);
    storeConst("TRUE", &primativeTypes[2], 1);
    storeConst("false", &primativeTypes[2], 0);
    storeConst("FALSE", &primativeTypes[2], 0);

    enterScope();
}

Constant SymbolTable::lookupConst(std::string id)
{
    for (auto curLayer = stack.rbegin(); curLayer != stack.rend(); curLayer++)
    {
        auto found = curLayer->constants.find(id);
        if(found != curLayer->constants.end())
        {
            return found->second;
        }
    }
    return {};
}

Variable SymbolTable::lookupVar(std::string id)
{
    for (auto curLayer = stack.rbegin(); curLayer != stack.rend(); curLayer++)
    {
        auto found = curLayer->variables.find(id);
        if(found != curLayer->variables.end())
        {
            return found->second;
        }
    }
    return {};
}

Type * SymbolTable::lookupType(std::string id)
{
    for (auto curLayer = stack.rbegin(); curLayer != stack.rend(); curLayer++)
    {
        auto found = curLayer->types.find(id);
        if(found != curLayer->types.end())
        {
            return found->second;
        }
    }
    return {};
}

void SymbolTable::storeType(std::string id, Type* t)
{
    auto topLayer = stack.rbegin();
    checkForIdDefined(id);
    topLayer->types[id] = t;
}

void SymbolTable::storeConst(std::string id, Type* t, int val)
{
    auto topLayer = stack.rbegin();
    checkForIdDefined(id);
    topLayer->constants[id] = t;
}

void SymbolTable::checkForIdDefined(std::string id)
{
    auto topLayer = stack.rbegin();
    if(topLayer->constants.find(id) != topLayer->constants.end())
    {
        throw std::runtime_error("Constant " + id + " is already defined in this scope");
    }
    if(topLayer->types.find(id) != topLayer->types.end())
    {
        throw std::runtime_error("Type " + id + " is already defined in this scope");
    }
}
    void storeVar(std::string, Type*, int, std::string);
    void enterScope();
    void leaveScope();