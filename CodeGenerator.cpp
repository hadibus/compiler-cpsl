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
            std::cout << "\tla $a0, (" << *re->getRegister() << ")" << std::endl;

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
        std::cout << "GA:" << std::endl;
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
            << "\taddi " << *reg << ", $0, " << fe->getValue() << std::endl
            << "\tsw " << *reg << ", " << lvale->getOffset() 
                << "(" << *lvale->getRegister() << ")" << std::endl;

        }
        else if (auto le = dynamic_cast<LvalExpression*>(expr))
        {
            auto reg = st.requestRegister();
            std::cout
            << "\tlw " << *reg << ", " << le->getOffset()
                << "(" << *le->getRegister() << ")" << std::endl
            << "\tsw " << *reg << ", " << lvale->getOffset()
                << "(" << *lvale->getRegister() << ")" << std::endl;
        }
        else if (auto re = dynamic_cast<RegisterExpression*>(expr))
        {
            std::cout
            << "\tsw " << *re->getRegister() << ", " << lvale->getOffset() 
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

    void CodeGenerator::storeConst(char* id, int i)
    {
        auto fe = dynamic_cast<FoldExpression*>(expressions[i]);
        if(!fe)
        {
            throw std::logic_error("error in const decls");
        }
        auto t = fe->getType();
        auto val = fe->getValue();
        st.storeConst(id, t, val);
    }

    int CodeGenerator::binOpAdd(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("Addition of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() + re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\taddi " << *reg << ", " << *reg << ", " << le->getValue() << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\taddi " << *re->getRegister() << ", " << *re->getRegister() << ", " << le->getValue()
                << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\taddi " << *reg << ", " << *reg << ", " << re->getValue() << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto regTemp = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *regTemp << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tadd " << *reg << ", " << *reg << ", " << *regTemp << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto tempReg = st.requestRegister();

                std::cout
                << "\tlw " << *tempReg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tadd " << *re->getRegister() << ", " << *re->getRegister() << ", " << *tempReg << std::endl;

                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                std::cout
                << "\taddi " << *le->getRegister() << ", " << *le->getRegister() << ", " << re->getValue()
                << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto tempReg = st.requestRegister();

                std::cout
                << "\tlw " << *tempReg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tadd " << *le->getRegister() << ", " << *le->getRegister() << ", " << *tempReg << std::endl;

                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tadd " << *le->getRegister() << ", " << *le->getRegister() << ", " << *re->getRegister()
                << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }

    int CodeGenerator::binOpAnd(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("And of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() & re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tandi " << *reg << ", " << *reg << ", " << le->getValue() << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tandi " << *re->getRegister() << ", " << *re->getRegister() << ", " << le->getValue()
                << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tandi " << *reg << ", " << *reg << ", " << re->getValue() << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto regTemp = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *regTemp << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tand " << *reg << ", " << *reg << ", " << *regTemp << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto tempReg = st.requestRegister();

                std::cout
                << "\tlw " << *tempReg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tand " << *re->getRegister() << ", " << *re->getRegister() << ", " << *tempReg << std::endl;

                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                std::cout
                << "\tandi " << *le->getRegister() << ", " << *le->getRegister() << ", " << re->getValue()
                << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto tempReg = st.requestRegister();

                std::cout
                << "\tlw " << *tempReg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tand " << *le->getRegister() << ", " << *le->getRegister() << ", " << *tempReg << std::endl;

                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tand " << *le->getRegister() << ", " << *le->getRegister() << ", " << *re->getRegister()
                << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }

    int CodeGenerator::binOpOr(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("Or of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() | re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tori " << *reg << ", " << *reg << ", " << le->getValue() << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tori " << *re->getRegister() << ", " << *re->getRegister() << ", " << le->getValue()
                << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tori " << *reg << ", " << *reg << ", " << re->getValue() << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto regTemp = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *regTemp << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tor " << *reg << ", " << *reg << ", " << *regTemp << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto tempReg = st.requestRegister();

                std::cout
                << "\tlw " << *tempReg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tor " << *re->getRegister() << ", " << *re->getRegister() << ", " << *tempReg << std::endl;

                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                std::cout
                << "\tori " << *le->getRegister() << ", " << *le->getRegister() << ", " << re->getValue()
                << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto tempReg = st.requestRegister();

                std::cout
                << "\tlw " << *tempReg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tor " << *le->getRegister() << ", " << *le->getRegister() << ", " << *tempReg << std::endl;

                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tor " << *le->getRegister() << ", " << *le->getRegister() << ", " << *re->getRegister()
                << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }

    int CodeGenerator::binOpEq(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("And of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() == re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();
                auto reg3 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << le->getValue() << std::endl
                << "\tslt " << *reg3 << ", " << *reg << ", " << *reg2 << std::endl
                << "\tslt " << *reg2 << ", " << *reg2 << ", " << *reg << std::endl
                << "\tnor " << *reg << ", " << *reg2 << ", " << *reg3 << std::endl;

                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tslt " << *reg2 << ", " << *reg << ", " << *re->getRegister() << std::endl
                << "\tslt " << *reg << ", " << *re->getRegister() << ", " << *reg << std::endl
                << "\tnor " << *re->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();
                auto reg3 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << re->getValue() << std::endl
                << "\tslt " << *reg3 << ", " << *reg << ", " << *reg2 << std::endl
                << "\tslt " << *reg2 << ", " << *reg2 << ", " << *reg << std::endl
                << "\tnor " << *reg << ", " << *reg2 << ", " << *reg3 << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();
                auto reg3 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tslt " << *reg3 << ", " << *reg << ", " << *reg2 << std::endl
                << "\tslt " << *reg2 << ", " << *reg2 << ", " << *reg << std::endl
                << "\tnor " << *reg << ", " << *reg2 << ", " << *reg3 << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();


                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tslt " << *reg2 << ", " << *reg << ", " << *re->getRegister() << std::endl
                << "\tslt " << *reg << ", " << *re->getRegister() << ", " << *reg << std::endl
                << "\tnor " << *re->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << re->getValue() << std::endl
                << "\tslt " << *reg2 << ", " << *reg << ", " << *le->getRegister() << std::endl
                << "\tslt " << *reg << ", " << *le->getRegister() << ", " << *reg << std::endl
                << "\tnor " << *le->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();


                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tslt " << *reg2 << ", " << *reg << ", " << *le->getRegister() << std::endl
                << "\tslt " << *reg << ", " << *le->getRegister() << ", " << *reg << std::endl
                << "\tnor " << *le->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tslt " << *reg << ", " << *le->getRegister() << ", " << *re->getRegister() << std::endl
                << "\tslt " << *re->getRegister() << ", " << *re->getRegister() << ", " << *le->getRegister() << std::endl
                << "\tnor " << *le->getRegister() << ", " << *reg << ", " << *re->getRegister() << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }

    int CodeGenerator::binOpNeq(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("And of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() != re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();
                auto reg3 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << le->getValue() << std::endl
                << "\tslt " << *reg3 << ", " << *reg << ", " << *reg2 << std::endl
                << "\tslt " << *reg2 << ", " << *reg2 << ", " << *reg << std::endl
                << "\tor " << *reg << ", " << *reg2 << ", " << *reg3 << std::endl;

                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tslt " << *reg2 << ", " << *reg << ", " << *re->getRegister() << std::endl
                << "\tslt " << *reg << ", " << *re->getRegister() << ", " << *reg << std::endl
                << "\tor " << *re->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();
                auto reg3 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << re->getValue() << std::endl
                << "\tslt " << *reg3 << ", " << *reg << ", " << *reg2 << std::endl
                << "\tslt " << *reg2 << ", " << *reg2 << ", " << *reg << std::endl
                << "\tor " << *reg << ", " << *reg2 << ", " << *reg3 << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();
                auto reg3 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tslt " << *reg3 << ", " << *reg << ", " << *reg2 << std::endl
                << "\tslt " << *reg2 << ", " << *reg2 << ", " << *reg << std::endl
                << "\tor " << *reg << ", " << *reg2 << ", " << *reg3 << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();


                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tslt " << *reg2 << ", " << *reg << ", " << *re->getRegister() << std::endl
                << "\tslt " << *reg << ", " << *re->getRegister() << ", " << *reg << std::endl
                << "\tor " << *re->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << re->getValue() << std::endl
                << "\tslt " << *reg2 << ", " << *reg << ", " << *le->getRegister() << std::endl
                << "\tslt " << *reg << ", " << *le->getRegister() << ", " << *reg << std::endl
                << "\tor " << *le->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();


                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tslt " << *reg2 << ", " << *reg << ", " << *le->getRegister() << std::endl
                << "\tslt " << *reg << ", " << *le->getRegister() << ", " << *reg << std::endl
                << "\tor " << *le->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tslt " << *reg << ", " << *le->getRegister() << ", " << *re->getRegister() << std::endl
                << "\tslt " << *re->getRegister() << ", " << *re->getRegister() << ", " << *le->getRegister() << std::endl
                << "\tor " << *le->getRegister() << ", " << *reg << ", " << *re->getRegister() << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }
    
    int CodeGenerator::binOpGteq(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("And of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() >= re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();
                auto reg3 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << le->getValue() << std::endl
                << "\tslt " << *reg << ", " << *reg << ", " << *reg2 << std::endl;

                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tslt " << *re->getRegister() << ", " << *re->getRegister() << ", " << *reg << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();
                auto reg3 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << re->getValue() << std::endl
                << "\tslt " << *reg << ", " << *reg2 << ", " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tslt " << *reg << ", " << *reg2 << ", " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tslt " << *re->getRegister() << ", " << *re->getRegister() << ", " << *reg << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << re->getValue() << std::endl
                << "\tslt " << *le->getRegister() << ", " << *reg << ", " << *le->getRegister() << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();


                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tslt " << *le->getRegister() << ", " << *reg << ", " << *le->getRegister() << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {

                std::cout
                << "\tslt " << *le->getRegister() << ", " << *re->getRegister() << ", " << *le->getRegister() << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }
    int CodeGenerator::binOpGt(int l, int r)
    {
        return binOpLteq(r,l);
    }
    int CodeGenerator::binOpLteq(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("And of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() >= re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << le->getValue() << std::endl
                << "\tslt " << *reg << ", " << *reg2 << ", " << *reg << std::endl;

                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tslt " << *re->getRegister() << ", " << *reg << ", " << *re->getRegister() <<  std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << re->getValue() << std::endl
                << "\tslt " << *reg << ", " << *reg << ", " << *reg2 << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tslt " << *reg << ", " << *reg << ", " << *reg2 << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tslt " << *re->getRegister() << ", " << *reg << ", " << *re->getRegister() << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << re->getValue() << std::endl
                << "\tslt " << *le->getRegister() << ", " << *le->getRegister() << ", " << *reg << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();


                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tslt " << *le->getRegister() << ", " << *le->getRegister() << ", " << *reg  << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {

                std::cout
                << "\tslt " << *le->getRegister() << ", " << *le->getRegister() << ", " << *re->getRegister() << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }
    int CodeGenerator::binOpLt(int l , int r )
    {
        return binOpGteq(r,l);
    }
    int CodeGenerator::binOpSub(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("Addition of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() - re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tli " << *reg2 << ", " << le->getValue() << std::endl
                << "\tsub " << *reg << ", " << *reg2 << ", " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tli " << *reg << ", " << le->getValue() << std::endl
                << "\tsub " << *re->getRegister() << ", " << *reg << ", " << *re->getRegister() << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tsubi " << *reg << ", " << *reg << ", " << re->getValue() << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto regTemp = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *regTemp << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tsub " << *reg << ", " << *reg << ", " << *regTemp << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto tempReg = st.requestRegister();

                std::cout
                << "\tlw " << *tempReg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tsub " << *re->getRegister() << ", " << *tempReg << ", " << *re->getRegister() << std::endl;

                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                std::cout
                << "\tsubi " << *le->getRegister() << ", " << *le->getRegister() << ", " << re->getValue()
                << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto tempReg = st.requestRegister();

                std::cout
                << "\tlw " << *tempReg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tsub " << *le->getRegister() << ", " << *le->getRegister() << ", " << *tempReg << std::endl;

                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tsub " << *le->getRegister() << ", " << *le->getRegister() << ", " << *re->getRegister()
                << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }
    int CodeGenerator::binOpMult(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("Addition of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() * re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\addi " << *reg2 << ", $0, " << le->getValue() << std::endl
                << "\tmult " << *reg << ", " << *reg2 << std::endl
                << "\tmflo " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tmult " << *reg << ", " << *re->getRegister() << std::endl
                << "\tmflo " << *re->getRegister() << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << re->getValue() << std::endl
                << "\tmult " << *reg << ", " << *reg2 << std::endl
                << "\tmflo " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tmult " << *reg << ", " << *reg2 << std::endl
                << "\tmflo " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tmult " << *re->getRegister() << ", " << *reg << std::endl
                << "\tmflo " << *re->getRegister() << std::endl;

                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << re->getValue() << std::endl
                << "\tmult " << *le->getRegister() << ", " << *reg << std::endl
                << "\tmflo " << *le->getRegister() << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tmult " << *le->getRegister() << ", " << *reg << std::endl
                << "\tmflo " << *le->getRegister() << std::endl;

                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tmult " << *le->getRegister() << ", " << *re->getRegister() << std::endl
                << "\tmflo " << *le->getRegister() << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }
    int CodeGenerator::binOpDiv(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("Addition of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() / re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\addi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tdiv " << *reg << ", " << *reg2 << std::endl
                << "\tmflo " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tdiv " << *reg << ", " << re->getRegister() << std::endl
                << "\tmflo " << re->getRegister() << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << re->getValue() << std::endl
                << "\tdiv " << *reg << ", " << *reg2 << std::endl
                << "\tmflo " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tdiv " << *reg << ", " << *reg2 << std::endl
                << "\tmflo " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tdiv " << *reg << ", " << *re->getRegister() << std::endl
                << "\tmflo " << *re->getRegister() << std::endl;

                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << re->getValue() << std::endl
                << "\tdiv " << *le->getRegister() << ", " << *reg << std::endl
                << "\tmflo " << *le->getRegister() << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tdiv " << *le->getRegister() << ", " << *reg << std::endl
                << "\tmflo " << *le->getRegister() << std::endl;

                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tdiv " << *le->getRegister() << ", " << *re->getRegister() << std::endl
                << "\tmflo " << *le->getRegister() << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }

    int CodeGenerator::binOpMod(int l, int r)
    {
        if (expressions[l]->getType() != expressions[r]->getType())
        {
            throw std::runtime_error("operation on distinct types is prohibited");
        }
        if(*(expressions[l]->getType()) == "string"
        || *(expressions[r]->getType()) == "string")
        {
            throw std::runtime_error("Addition of string is prohibited");
        }

        if(auto le = dynamic_cast<FoldExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                le->setValue(le->getValue() % re->getValue());
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\addi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tdiv " << *reg << ", " << *reg2 << std::endl
                << "\tmfhi " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(le->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << le->getValue() << std::endl
                << "\tdiv " << *reg << ", " << re->getRegister() << std::endl
                << "\tmfhi " << re->getRegister() << std::endl;
                return r;
            }
        }
        else
        if(auto le = dynamic_cast<LvalExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\taddi " << *reg2 << ", $0, " << re->getValue() << std::endl
                << "\tdiv " << *reg << ", " << *reg2 << std::endl
                << "\tmfhi " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();
                auto reg2 = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tlw " << *reg2 << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tdiv " << *reg << ", " << *reg2 << std::endl
                << "\tmfhi " << *reg << std::endl;
                
                auto regExpr = new RegisterExpression();
                regExpr->setRegister(reg);
                regExpr->setType(re->getType());
                expressions.push_back(regExpr);
                return expressions.size() - 1;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")" << std::endl
                << "\tdiv " << *reg << ", " << *re->getRegister() << std::endl
                << "\tmfhi " << *re->getRegister() << std::endl;

                return r;
            }
        }
        else
        if(auto le = dynamic_cast<RegisterExpression*>(expressions[l]))
        {

            if(auto re = dynamic_cast<FoldExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\taddi " << *reg << ", $0, " << re->getValue() << std::endl
                << "\tdiv " << *le->getRegister() << ", " << *reg << std::endl
                << "\tmfhi " << *le->getRegister() << std::endl;
                return l;
            }
            else
            if(auto re = dynamic_cast<LvalExpression*>(expressions[r]))
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tlw " << *reg << ", " << re->getOffset() << "(" << *re->getRegister() << ")" << std::endl
                << "\tdiv " << *le->getRegister() << ", " << *reg << std::endl
                << "\tmfhi " << *le->getRegister() << std::endl;

                return l;
            }
            else
            if(auto re = dynamic_cast<RegisterExpression*>(expressions[r]))
            {
                std::cout
                << "\tdiv " << *le->getRegister() << ", " << *re->getRegister() << std::endl
                << "\tmfhi " << *le->getRegister() << std::endl;

                re->releaseRegister();
                return l;
            }
        }
        else
        {
            throw std::logic_error("operation failure");
        }
        
    }

    int CodeGenerator::unOpNeg(int i)
    {
        if (*expressions[i]->getType() != "integer")
        {
            throw std::runtime_error("Non integer negation");
        }
        if (auto e = dynamic_cast<FoldExpression*>(expressions[i]))
        {
            e->setValue(e->getValue() * -1);
            return i;
        }
        else
        if (auto e = dynamic_cast<LvalExpression*>(expressions[i]))
        {
            auto reg = st.requestRegister();
            auto reg2 = st.requestRegister();

            std::cout
            << "\tlw " << *reg << ", " << e->getOffset() << "(" << e->getRegister() << ")" << std::endl
            << "\taddi " << *reg2 << ", $0, -1" << std::endl
            << "\tmult " << *reg << ", " << *reg2 << std::endl
            << "\tmflo " << *reg << std::endl;

            auto regExpr = new RegisterExpression();
            regExpr->setRegister(reg);
            regExpr->setType(e->getType());
            expressions.push_back(regExpr);
            return expressions.size() - 1;
        }
        else
        if (auto e = dynamic_cast<RegisterExpression*>(expressions[i]))
        {
            auto reg = st.requestRegister();

            std::cout 
            << "\taddi " << *reg << ", $0, -1" << std::endl
            << "\tmult " << *e->getRegister() << ", " << *reg << std::endl
            << "\tmflo " << *e->getRegister() << std::endl;

            return i;
        }
    }
    int CodeGenerator::unOpNot(int i)
    {
        if (*expressions[i]->getType() != "integer"
         && *expressions[i]->getType() != "boolean")
        {
            throw std::runtime_error("Negation is allowed only for integer and boolean datatypes");
        }
        if (auto e = dynamic_cast<FoldExpression*>(expressions[i]))
        {
            switch(e->getValue())
            {
            case 0:
                e->setValue(1);
                break;
            default:
                e->setValue(0);
            }
            e->setType(st.getPrimativeType("boolean"));
            return i;
        }
        else
        if (auto e = dynamic_cast<LvalExpression*>(expressions[i]))
        {
            auto reg = st.requestRegister();
            auto reg2 = st.requestRegister();
            auto reg3 = st.requestRegister();

            std::cout 
            << "\tlw " << *reg << ", " << e->getOffset() << "(" << e->getRegister() << ")" << std::endl
            << "\tslti " << *reg2 << ", " << *reg << ", 1" << std::endl
            << "\tslti " << *reg3 << ", " << *reg << ", 0" << std::endl
            << "\txor " << *reg << ", " << *reg2 << ", " << *reg3 << std::endl;

            auto regExpr = new RegisterExpression();
            regExpr->setRegister(reg);
            regExpr->setType(st.getPrimativeType("boolean"));
            expressions.push_back(regExpr);
            return expressions.size() - 1;
        }
        else
        if (auto e = dynamic_cast<RegisterExpression*>(expressions[i]))
        {
            auto reg = st.requestRegister();
            auto reg2 = st.requestRegister();

            std::cout
            << "\tslti " << *reg << ", " << e->getRegister() << ", 1" << std::endl
            << "\tslti " << *reg2 << ", " << e->getRegister() << ", 0" << std::endl
            << "\txor " << e->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;

            e->setType(st.getPrimativeType("boolean"));

            return i;
        }
        throw std::logic_error("Unknown expression type");
    }


    int CodeGenerator::unOpDecr(int i)
    {
        if (*expressions[i]->getType() == "string")
        {
            throw std::runtime_error("Decrement of string datatype is prohibited");
        }

        if (*expressions[i]->getType() == "boolean")
        {
            return unOpNeg(i);
        }

        if (auto e = dynamic_cast<FoldExpression*>(expressions[i]))
        {
            e->setValue(e->getValue() - 1);
            return i;
        }
        else
        if (auto e = dynamic_cast<LvalExpression*>(expressions[i]))
        {
            auto reg = st.requestRegister();

            std::cout 
            << "\tlw " << *reg << ", " << e->getOffset() << "(" << e->getRegister() << ")" << std::endl
            << "\taddi " << *reg << ", " << *reg << ", -1" << std::endl;

            auto regExpr = new RegisterExpression();
            regExpr->setRegister(reg);
            regExpr->setType(e->getType());
            expressions.push_back(regExpr);
            return expressions.size() - 1;
        }
        else
        if (auto e = dynamic_cast<RegisterExpression*>(expressions[i]))
        {
            std::cout
            << "\taddi " << e->getRegister() << ", " << *e->getRegister() << ", -1" << std::endl;

            return i;
        }
    }

    int CodeGenerator::unOpIncr(int i)
    {
        if (*expressions[i]->getType() == "string")
        {
            throw std::runtime_error("Increment of string datatype is prohibited");
        }

        if (*expressions[i]->getType() == "boolean")
        {
            return unOpNeg(i);
        }

        if (auto e = dynamic_cast<FoldExpression*>(expressions[i]))
        {
            e->setValue(e->getValue() + 1);
            return i;
        }
        else
        if (auto e = dynamic_cast<LvalExpression*>(expressions[i]))
        {
            auto reg = st.requestRegister();

            std::cout 
            << "\tlw " << *reg << ", " << e->getOffset() << "(" << e->getRegister() << ")" << std::endl
            << "\taddi " << *reg << ", " << *reg << ", 1" << std::endl;

            auto regExpr = new RegisterExpression();
            regExpr->setRegister(reg);
            regExpr->setType(e->getType());
            expressions.push_back(regExpr);
            return expressions.size() - 1;
        }
        else
        if (auto e = dynamic_cast<RegisterExpression*>(expressions[i]))
        {
            std::cout
            << "\taddi " << e->getRegister() << ", " << *e->getRegister() << ", 1" << std::endl;

            return i;
        }
    }