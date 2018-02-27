#include <iostream>

#include "CodeGenerator.hpp"
#include "Value.hpp"

    Value CodeGenerator::charLiteral(char c)
    {
        Value val;
        val.value = c;
        val.type = st.getPrimativeType("char");
        return val;
    }

    void CodeGenerator::assertIntOrChar(Value v)
    {
        auto charType = st.getPrimativeType("char");
        auto intType = st.getPrimativeType("integer");
        if (v.type != charType && v.type != intType)
        {
                throw std::runtime_error("invalid conversion");
        }
    }

    Value CodeGenerator::charCast(Value v)
    {
        assertIntOrChar(v);
        v.type = st.getPrimativeType("char");
        return v;
    }

    Value CodeGenerator::intLiteral(int i)
    {
        Value val;
        val.value = i;
        val.type = st.getPrimativeType("integer");
        return val;
    }

    Value CodeGenerator::intCast(Value v)
    {
        assertIntOrChar(v);
        v.type = st.getPrimativeType("char");
        return v; 
    }

    int CodeGenerator::stringLiteral(char* cstr)
    {
        Value val;
        val.value = st.storeStringLiteral(cstr);
        val.type = st.getPrimativeType("string");
        values.push_back(val);
        return values.size() -1;
    }
    void CodeGenerator::writeExpression()
    {
        auto val = values.end() -1;
        values.erase(val);
        if (val->type == st.getPrimativeType("string"))
        {
            std::cout
            << "\tla $a0, STR" << val->value << std::endl
            << "\tli $v0, 4" << std::endl
            << "\tsyscall" << std::endl;
        }
        else
        {
            throw std::runtime_error("This type aint defined!");
        }
    }


    void CodeGenerator::printHeader()
    {
        std::cout
        << ".text" << std::endl
        << ".globl main" << std::endl
        << std::endl
        << "main:" << std::endl
	    << "\tla $gp, GA" << std::endl
	    << "\tj real_main" << std::endl
        << std::endl
        << "real_main:" << std::endl;
    }

    void CodeGenerator::printFooter()
    {
        doStop();
        std::cout
        << ".data" << std::endl
        << ".asciiz" << std::endl;
        auto sl = st.getStringList();
        for (auto i = 0U; i < sl.size(); i++)
        {
            std::cout << "STR" << i << ": " << sl[i] << std::endl;
        }
        std::cout << "GA:" << std::endl;
    }

    void CodeGenerator::doStop()
    {
        std::cout 
        << "\tli $a0, 0" << std::endl
        << "\tli $v0, 17" << std::endl
        << "\tsyscall" << std::endl
        << std::endl;
    }

   /*
    int CodeGenerator::binOpAdd(int, int);
    int CodeGenerator::binOpAnd(int, int);
    int CodeGenerator::binOpDiv(int, int);
    int CodeGenerator::binOpEq(int, int);
    int CodeGenerator::binOpGteq(int, int);
    int CodeGenerator::binOpGt(int, int);
    int CodeGenerator::binOpLteq(int, int);
    int CodeGenerator::binOpLt(int, int);
    int CodeGenerator::binOpMod(int, int);
    int CodeGenerator::binOpMult(int, int);
    int CodeGenerator::binOpNeq(int, int);
    int CodeGenerator::binOpOr(int, int);
    int CodeGenerator::binOpSub(int, int);

    int CodeGenerator::unOpNeg(int);
    int CodeGenerator::unOpNot(int);
    int CodeGenerator::unOpDecr(int);
    int CodeGenerator::unOpIncr(int);
    */