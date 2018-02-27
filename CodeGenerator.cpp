#include <iostream>

#include "CodeGenerator.hpp"
#include "FoldExpression.hpp"
#include "RegisterExpression.hpp"

    int CodeGenerator::charLiteral(char c)
    {
        FoldExpression *fe = new FoldExpression();
        fe->setType(st.getPrimativeType("char"));
        fe->setValue(c);
        expressions.push_back(fe);
        return expressions.size() - 1;
    }

    void CodeGenerator::assertIntOrChar(int i)
    {
        auto e = expressions[i];
        auto charType = st.getPrimativeType("char");
        auto intType = st.getPrimativeType("integer");
        if (e->getType() != charType && e->getType() != intType)
        {
                throw std::runtime_error("invalid conversion");
        }
    }

    int CodeGenerator::charCast(int i)
    {
        assertIntOrChar(i);
        auto e = expressions[i];
        e->setType(st.getPrimativeType("char"));
        return i;
    }

    int CodeGenerator::intLiteral(int i)
    {
        FoldExpression *fe = new FoldExpression();
        fe->setValue(i);
        fe->setType(st.getPrimativeType("integer"));
        expressions.push_back(fe);
        return expressions.size() - 1;
    }

    int CodeGenerator::intCast(int i)
    {
        assertIntOrChar(i);
        auto e = expressions[i];
        e->setType(st.getPrimativeType("integer"));
        return i; 
    }

    int CodeGenerator::stringLiteral(char* cstr)
    {
        FoldExpression *fe = new FoldExpression();
        fe->setType(st.getPrimativeType("string"));
        fe->setValue(st.storeStringLiteral(cstr));
        expressions.push_back(fe);
        return expressions.size() - 1;
    }
    void CodeGenerator::writeExpression(int i)
    {
        auto e = expressions[i];
        if(auto fe = dynamic_cast<FoldExpression*>(e))
        {
            if (fe->getType() == st.getPrimativeType("string"))
            {
                std::cout
                << "\tla $a0, STR" << fe->getValue() << std::endl
                << "\tli $v0, 4" << std::endl
                << "\tsyscall" << std::endl;
            }
            else if (fe->getType() == st.getPrimativeType("char"))
            {
                std::cout
                << "\tla $a0, " << fe->getValue() << std::endl
                << "\tli $v0, 11" << std::endl
                << "\tsyscall" << std::endl;
            }
            else if (fe->getType() == st.getPrimativeType("integer")
                  || fe->getType() == st.getPrimativeType("boolean"))
            {
                std::cout
                << "\tla $a0, " << fe->getValue() << std::endl
                << "\tli $v0, 1" << std::endl
                << "\tsyscall" << std::endl;
            }
            else
            {
                throw std::logic_error("This type aint defined!");
            }

        }
        else if (auto re = dynamic_cast<RegisterExpression*>(e))
        {
            //TODO work with register values.
        }
        else
        {
            throw std::logic_error("dynamic cast didn't work");
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

    int CodeGenerator::storeType(char* id, int i)
    {
        auto type = st.getPrimativeType(i);
        st.storeType(id, type);
    }

    int CodeGenerator::lookupType(char* c)
    {
        return st.lookupType(c);
    }

    void CodeGenerator::clearExpressions()
    {
        for (auto & e : expressions)
        {
            delete e;
            e = nullptr;
        }
        expressions.clear();
    }

    void CodeGenerator::appendStrList(char* c)
    {
        tempStrList.push_back(c);
    }

    void CodeGenerator::makeVars(int i, std::string reg)
    {
        auto type = st.getPrimativeType(i);
        for (const auto & name : tempStrList)
        {
            st.storeVar(name, type, reg);
        }
        tempStrList.clear();
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