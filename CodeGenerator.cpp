#include <iostream>

#include "CodeGenerator.hpp"
#include "FoldExpression.hpp"
#include "LvalExpression.hpp"

const unsigned STRING_VAR_SIZE = 64;

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
        fe->setType(st.getPrimativeType("integer"));
        fe->setValue(i);
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
        else if (auto le = dynamic_cast<LvalExpression*>(e))
        {
            auto reg = st.requestRegister();
            std::cout
            << "\tlw " << *reg << ", " << le->getOffset() 
                << "(" << *le->getRegister() << ")" << std::endl
            << "\tla $a0, (" << *reg << ")" << std::endl;

            if (le->getType() == st.getPrimativeType("string"))
            {
                std::cout << "\tli $v0, 4" << std::endl;
            }
            else if (le->getType() == st.getPrimativeType("char"))
            {
                std::cout << "\tli $v0, 11" << std::endl;
            }
            else if (le->getType() == st.getPrimativeType("integer")
                  || le->getType() == st.getPrimativeType("boolean"))
            {
                std::cout << "\tli $v0, 1" << std::endl;
            }
            else
            {
                throw std::logic_error("This type aint defined!");
            }
            std::cout << "\tsyscall" << std::endl;

        }
        else if (auto re = dynamic_cast<RegisterExpression*>(e))
        {
            std::cout << "\tla $a0, " << *re->getRegister() << std::endl;

            if (re->getType() == st.getPrimativeType("string"))
            {
                std::cout << "\tli $v0, 4" << std::endl;
            }
            else if (re->getType() == st.getPrimativeType("char"))
            {
                std::cout << "\tli $v0, 11" << std::endl;
            }
            else if (re->getType() == st.getPrimativeType("integer")
                  || re->getType() == st.getPrimativeType("boolean"))
            {
                std::cout << "\tli $v0, 1" << std::endl;
            }
            else
            {
                throw std::logic_error("This type aint defined!");
            }
            std::cout << "\tsyscall" << std::endl;

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
        auto vsc = st.getVarStrCount();
        for (auto i = 0U; i < vsc; i++)
        {
            std::cout << "VAR_STR" << i << ": .space " << STRING_VAR_SIZE << std::endl;
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

    int CodeGenerator::getLval(std::string lval)
    {
        auto c = st.lookupConst(lval);
        auto v = st.lookupVar(lval);
        if (c.type == nullptr && v.reg == "")
        {
            throw std::runtime_error("lval " + lval + " is not defined");
        }
        else if (c.type != nullptr)
        {
            FoldExpression *fe = new FoldExpression();
            fe->setValue(c.value);
            fe->setType(c.type);
            expressions.push_back(fe);
            return expressions.size() - 1;
        }
        else if (v.reg != "")
        {
            LvalExpression *le = new LvalExpression();
            le->setRegister(std::make_shared<std::string>(v.reg));
            le->setOffset(v.offset);
            std::cerr << "getLval " << le->getOffset() << std::endl;
            le->setType(v.type);
            expressions.push_back(le);
            return expressions.size() - 1;
        }
        else
        {
            throw std::logic_error("Lval" + lval + " is a const and variable");
        }

    }

    int CodeGenerator::assignExprToLval(int li, int ei)
    {
        auto lvale = dynamic_cast<LvalExpression*>(expressions[li]);
        auto expr = expressions[ei];

        if(!lvale)
        {
            throw std::logic_error("lval is not RegisterExpression");
        }
        if (lvale->getType() != expr->getType())
        {
            throw std::runtime_error("Types do not match");
        }

        if (auto fe = dynamic_cast<FoldExpression*>(expr))
        {
            auto reg = st.requestRegister();
            std::cout
            << "\tli " << *reg << ", " << fe->getValue() << std::endl
            << "\tsw " << *reg << ", " << lvale->getOffset() 
                << "(" << *lvale->getRegister() << ")" << std::endl;

        }
        else if (auto re = dynamic_cast<RegisterExpression*>(expr))
        {
            std::cout
            << "\tsw" << *re->getRegister() << ", " << lvale->getOffset() 
                << "(" << *lvale->getRegister() << ")" << std::endl;
        }
        else
        {
            throw std::logic_error("assignment expression bad");
        }

        return ei;

    }

    void CodeGenerator::readToLval(int i)
    {
        auto le = dynamic_cast<LvalExpression*>(expressions[i]);
        if (!le)
        {
            throw std::logic_error("lval not modifiable");
        }

        /*
        read int 5
        read string 8
        read character 12
        */
        if (le->getType() == st.getPrimativeType("integer")
         || le->getType() == st.getPrimativeType("boolean"))
        {
            std::cout << "\tli $v0, 5" << std::endl
            << "\tsyscall" << std::endl
            << "\tsw $v0, " << le->getOffset() << "(" << *le->getRegister() << ")" 
            << std::endl;

        }
        else if (le->getType() == st.getPrimativeType("char"))
        {
            std::cout << "\tli $v0, 12" << std::endl
            << "\tsyscall" << std::endl
            << "\tsw $v0, " << le->getOffset() << "(" << *le->getRegister() << ")" 
            << std::endl;
        }
        else if (le->getType() == st.getPrimativeType("string"))
        {
            std::cout
            << "\tla $a0, VAR_STR" << le->getOffset() << std::endl
            << "\tli $a1, " << STRING_VAR_SIZE << std::endl
            << "\tli $v0, 8" << std::endl;
        }
        else
        {
            throw std::logic_error("In read, value type is bad");
        }
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