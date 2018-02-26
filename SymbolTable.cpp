#include <stdexcept>
#include <algorithm>

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
    checkForIdDefined(id);
    auto topLayer = stack.rbegin();
    topLayer->types[id] = t;
}

void SymbolTable::storeConst(std::string id, Type* t, int val)
{
    checkForIdDefined(id);
    auto topLayer = stack.rbegin();
    Constant c;
    c.type = t;
    c.value = val;
    topLayer->constants[id] = c;
}

void SymbolTable::storeVar(std::string id, Type* t, int, std::string reg)
{
    checkForIdDefined(id);
    auto topLayer = stack.rbegin();
    Variable v;
    v.offset = offset;
    offset += 4;
    v.reg = reg;
    v.type = t;
    topLayer->variables[id] = v;
}

int SymbolTable::storeStringLiteral(std::string s)
{
    stringList.push_back(s);
    return(stringList.size() - 1);
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

void SymbolTable::enterScope()
{
    stack.emplace_back();
}

void SymbolTable::leaveScope()
{
    stack.pop_back();
}

Type* SymbolTable::getPrimativeType(std::string s)
{
    auto found = std::find(primativeTypes.begin(), primativeTypes.end(), s);
    if (found == primativeTypes.end())
    {
        throw std::domain_error("Primative type " + s + " not found");
    }
    return &(*found);
}

std::vector<std::string> getStringList()
{
    return stringList;
}