#include <algorithm>
#include <iostream>

#include "ArrayType.hpp"
#include "BooleanType.hpp"
#include "CharacterType.hpp"
#include "IntegerType.hpp"
#include "StringType.hpp"
#include "SymbolTable.hpp"

extern void yyerror(const char*);


unsigned const INT_TYPE = 0, CHAR_TYPE = 1, BOOL_TYPE = 2, STR_TYPE = 3;

void SymbolTable::initialize()
{
    primitiveTypes.push_back(new IntegerType);
    primitiveTypes.push_back(new CharacterType);
    primitiveTypes.push_back(new BooleanType);
    primitiveTypes.push_back(new StringType);

    enterScope();
    storeType("integer", primitiveTypes[INT_TYPE]);
    storeType("INTEGER", primitiveTypes[INT_TYPE]);
    storeType("char", primitiveTypes[CHAR_TYPE]);
    storeType("CHAR", primitiveTypes[CHAR_TYPE]);
    storeType("boolean", primitiveTypes[BOOL_TYPE]);
    storeType("BOOLEAN", primitiveTypes[BOOL_TYPE]);
    storeType("string", primitiveTypes[STR_TYPE]);
    storeType("STRING", primitiveTypes[STR_TYPE]);

    storeConst("true", primitiveTypes[BOOL_TYPE], 1);
    storeConst("TRUE", primitiveTypes[BOOL_TYPE], 1);
    storeConst("false", primitiveTypes[BOOL_TYPE], 0);
    storeConst("FALSE", primitiveTypes[BOOL_TYPE], 0);

    enterScope();
    pushFrameOffset();

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
            auto foundInIneff = std::find(
                ineffableTypes.begin(),
                ineffableTypes.end(),
                found->second);
            if (foundInIneff != ineffableTypes.end())
            {
                return foundInIneff - ineffableTypes.begin();
            }
            
            for (auto idx = 0U; idx < primitiveTypes.size(); idx++)
            {
                if (primitiveTypes[idx] == found->second)
                {
                    ineffableTypes.push_back(primitiveTypes[idx]);
                    return ineffableTypes.size() - 1;
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

void SymbolTable::storeVar(std::string id, Type* t, std::string reg, bool onStack)
{
    checkForIdDefined(id);
    auto topLayer = stack.rbegin();
    Variable v;
    if (t == getPrimitiveType("string"))
    {
        stringList.emplace_back();
        v.offset = stringList.size() - 1;
    }
    else
    {
        if (onStack)
        {
            frameOffsets.back() -= t->getSizeRecursive();
            v.offset = frameOffsets.back();
        }
        else
        {
            v.offset = offset;
            offset += t->getSizeRecursive();
        }
    }
    v.reg = reg;
    v.type = t;
    topLayer->variables[id] = v;
}

void SymbolTable::storeParam(std::string id,Type* t)
{
    auto v = Variable();
    v.reg = "$fp";
    v.type = t;
    v.offset = offset;
    offset += t->getSizeRecursive();
    currFunc->parameters[id] = v ;
    currFunc->paramNames.push_back(id);
}

void SymbolTable::changeFrameOffsetBy(int i)
{
    frameOffsets.back() += i;
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

Type* SymbolTable::getPrimitiveType(std::string s)
{
    if (s == "boolean")     return primitiveTypes[BOOL_TYPE];
    if (s == "integer")     return primitiveTypes[INT_TYPE];
    if (s == "character")   return primitiveTypes[CHAR_TYPE];
    if (s == "string")      return primitiveTypes[STR_TYPE];
    throw std::domain_error("Primitive type " + s + " not found");
}

Type* SymbolTable::getPrimitiveType(int i)
{
    return primitiveTypes[i];
}

int SymbolTable::addIneffableType(Type *t)
{
    ineffableTypes.push_back(t);
    return ineffableTypes.size() - 1;
}

Type * SymbolTable::getIneffableType(int i)
{
    return ineffableTypes[i];
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
    yyerror("Depleted register pool");
}

void SymbolTable::changeVarOffset(std::string id, int offset)
{
    for (auto curLayer = stack.rbegin(); curLayer != stack.rend(); curLayer++)
    {
        auto found = curLayer->variables.find(id);
        if(found != curLayer->variables.end())
        {
            found->second.offset = offset;
            return;
        }
    }
}

void SymbolTable::pushFrameOffset()
{
    frameOffsets.push_back(0);
}

void SymbolTable::popFrameOffset()
{
    frameOffsets.pop_back();
}