#include <algorithm>
#include <iostream>

#include "ArrayType.hpp"
#include "BooleanType.hpp"
#include "CharacterType.hpp"
#include "IntegerType.hpp"
#include "StringType.hpp"
#include "SymbolTable.hpp"

extern void yyerror(const char*);

unsigned const GLOBAL_PLACE = 0;
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
}

Constant SymbolTable::lookupConst(std::string id)
{
    auto lookItUp = [](const Frame &f, const std::string &id)
    {
        const auto found = f.constants.find(id);
        if (found != f.constants.end()) return found->second;
        return Constant();
    };

    auto v = lookItUp(stack.back(), id);
    if (v.type != nullptr) return v;

    return lookItUp(stack[GLOBAL_PLACE], id);
}

Variable SymbolTable::lookupVar(std::string id)
{
    auto lookItUp = [](const Frame &f, const std::string &id)
    {
        for (auto i = f.forVars.rbegin(); i != f.forVars.rend(); i++)
        {
            if (i->first == id)
            {
                return i->second;
            }
        }
        const auto found = f.variables.find(id);
        if (found != f.variables.end())
        {
            return found->second;
        }
        return Variable();
    };

    auto v = lookItUp(*stack.rbegin(), id);
    if (v.reg != "") return v;
    return lookItUp(stack[GLOBAL_PLACE], id);
};

int SymbolTable::lookupType(std::string id)
{
    int ret = -1;
    auto lookItUp = [&](const Frame &f, const std::string &id)
    {
        auto found = f.types.find(id);
        if(found != f.types.end())
        {
            auto foundInEneff = std::find(
                ineffableTypes.begin(),
                ineffableTypes.end(),
                found->second
            );
            if(foundInEneff != ineffableTypes.end())
            {
                return static_cast<int>(foundInEneff - ineffableTypes.begin());
            }

            for (auto idx = 0U; idx < primitiveTypes.size(); idx++)
            {
                if (primitiveTypes[idx] == found->second)
                {
                    ineffableTypes.push_back(primitiveTypes[idx]);
                    return static_cast<int>(ineffableTypes.size() - 1);
                }
            }
        }
        return -1;
    };

    if (!stack.empty()) ret = lookItUp(stack.back(),id);
    if (ret == -1) ret = lookItUp(stack[GLOBAL_PLACE],id);
    if (ret == -1) throw std::runtime_error("Type " + id + " not defined0");
    return ret;
}

Type * SymbolTable::getType(std::string id)
{
    if(!stack.empty())
    {
        auto found = stack.back().types.find(id);
        if (found != stack.back().types.end())
        {
            return found->second;
        }
    }
    auto found = stack[GLOBAL_PLACE].types.find(id);
    if (found != stack[GLOBAL_PLACE].types.end())
    {
        return found->second;
    }
    throw std::runtime_error("Type " + id + " not defined");
}

void SymbolTable::storeType(std::string id, Type* t)
{
        stack.back().types[id] = t; //will be global when needed
}

void SymbolTable::storeConst(std::string id, Type* t, int val)
{
    checkForIdDefined(id);
    std::map<std::string,Constant> * topLayer = &stack[GLOBAL_PLACE].constants;
    if (!stack.empty()) topLayer = &(stack.back().constants);
    Constant c;
    c.type = t;
    c.value = val;
    (*topLayer)[id] = c;
}

void SymbolTable::storeVar(std::string id, Type* t)
{
    checkForIdDefined(id);
    Frame &f = stack[GLOBAL_PLACE];
    if (!stack.empty()) f = stack.back();
   
    Variable v;
    if (t == getPrimitiveType("string"))
    {
        stringList.emplace_back();
        v.offset = stringList.size() - 1;
    }
    else
    {
        if (!stack.empty())
        {
            stack.back().offset -= t->getSizeRecursive();
            v.offset = stack.back().offset;
        }
        else
        {
            v.offset = globalFrame.offset;
            globalFrame.offset += t->getSizeRecursive();
        }
    }
    v.reg = reg;
    v.type = t;
    f.variables[id] = v;
    

    
}

void SymbolTable::changeFrameOffsetBy(int i)
{
    stack.back().offset += i;
}

int SymbolTable::storeStringLiteral(std::string s)
{
    stringList.push_back(s);
    return(stringList.size() - 1);
}


void SymbolTable::checkForIdDefined(std::string id)
{
    auto checky = [&](const Frame &f)
    {
        if(f.constants.find(id) != f.constants.end())
        {
            throw std::runtime_error("Constant " + id + " is already defined in this scope");
        }
        if(f.variables.find(id) != f.variables.end())
        {
            throw std::runtime_error("Variable " + id + " is already defined in this scope");
        }
    };
    if(!stack.empty()) checky(stack.back());
    checky(globalFrame);
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
    std::vector<std::shared_ptr<std::string>> *regPool = &(globalFrame.regPool);
    if (!stack.empty()) regPool = &(stack.back().regPool);
    for (auto & reg : *regPool)
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