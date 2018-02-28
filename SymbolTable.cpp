#include <stdexcept>
#include <algorithm>
#include <iostream>

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

    const int NUM_T_REGS = 10; // 0-9
    for(auto i = 0; i < NUM_T_REGS; i++)
    {
        regPool.push_back(std::make_shared<std::string>("$t" + std::to_string(i)));
    }

    const int NUM_S_REGS = 8; // 0-7
    for(auto i = 0; i < NUM_S_REGS; i++)
    {
        regPool.push_back(std::make_shared<std::string>("$s" + std::to_string(i)));
    }
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

int SymbolTable::lookupType(std::string id)
{
    for (auto curLayer = stack.rbegin(); curLayer != stack.rend(); curLayer++)
    {
        auto found = curLayer->types.find(id);
        if(found != curLayer->types.end())
        {
            for (auto idx = 0U; idx < primativeTypes.size(); idx++)
            {
                if (&primativeTypes[idx] == found->second)
                {
                    return idx;
                }
            }
        }
    }
    throw std::runtime_error("Type " + id + " not defined");
}

Type * SymbolTable::getType(std::string id)
{
    for (auto curLayer = stack.rbegin(); curLayer != stack.rend(); curLayer++)
    {
        auto found = curLayer->types.find(id);
        if(found != curLayer->types.end())
        {
            return found->second;
        }
    }
    throw std::runtime_error("Type " + id + " not defined");
}

void SymbolTable::storeType(std::string id, Type* t)
{
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

void SymbolTable::storeVar(std::string id, Type* t, std::string reg)
{
    checkForIdDefined(id);
    auto topLayer = stack.rbegin();
    Variable v;
    if (t == getPrimativeType("string"))
    { // string gets special treatment. space to be made in footer
        v.offset = varStrCount;
        varStrCount++;
    }
    else
    {
        v.offset = offset;
        offset += 4;
    }
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
    if(topLayer->variables.find(id) != topLayer->variables.end())
    {
        throw std::runtime_error("Variable " + id + " is already defined in this scope");
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

Type* SymbolTable::getPrimativeType(int i)
{
    return &primativeTypes[i];
}

std::vector<std::string> SymbolTable::getStringList()
{
    return stringList;
}

std::shared_ptr<std::string> SymbolTable::requestRegister()
{
    for (auto & reg : regPool)
    {
        if(reg.unique())
        {
            return reg;
        }
    }
    throw std::logic_error("Depleted register pool");
}