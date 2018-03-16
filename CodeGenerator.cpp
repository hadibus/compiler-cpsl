#include <iostream>

#include "CodeGenerator.hpp"
#include "FoldExpression.hpp"
#include "LvalExpression.hpp"
#include "BooleanType.hpp"
#include "CharacterType.hpp"
#include "IntegerType.hpp"
#include "StringType.hpp"

const unsigned STRING_VAR_SIZE = 64;

    int CodeGenerator::charLiteral(char c)
    {
        FoldExpression *fe = new FoldExpression();
        fe->setType(st.getPrimitiveType("character"));
        fe->setValue(c);
        expressions.push_back(fe);
        return expressions.size() - 1;
    }

    void CodeGenerator::assertIntOrChar(int i)
    {
        auto e = expressions[i];
        auto charType = st.getPrimitiveType("character");
        auto intType = st.getPrimitiveType("integer");
        if (e->getType() != charType && e->getType() != intType)
        {
                throw std::runtime_error("invalid conversion");
        }
    }

    int CodeGenerator::charCast(int i)
    {
        assertIntOrChar(i);
        auto e = expressions[i];
        e->setType(st.getPrimitiveType("character"));
        return i;
    }

    int CodeGenerator::intLiteral(int i)
    {
        FoldExpression *fe = new FoldExpression();
        fe->setType(st.getPrimitiveType("integer"));
        fe->setValue(i);
        expressions.push_back(fe);
        return expressions.size() - 1;
    }

    int CodeGenerator::intCast(int i)
    {
        assertIntOrChar(i);
        auto e = expressions[i];
        e->setType(st.getPrimitiveType("integer"));
        return i; 
    }

    int CodeGenerator::stringLiteral(char* cstr)
    {
        FoldExpression *fe = new FoldExpression();
        fe->setType(st.getPrimitiveType("string"));
        fe->setValue(st.storeStringLiteral(cstr));
        expressions.push_back(fe);
        return expressions.size() - 1;
    }

    void CodeGenerator::writeExpression(int i)
    {
        auto e = expressions[i];
        if(auto fe = dynamic_cast<FoldExpression*>(e))
        {
            if (fe->getType() == st.getPrimitiveType("string"))
            {
                std::cout
                << "\tla $a0, STR" << fe->getValue() << std::endl
                << "\tli $v0, 4" << std::endl
                << "\tsyscall" << std::endl;
            }
            else if (fe->getType() == st.getPrimitiveType("character"))
            {
                std::cout
                << "\tla $a0, " << fe->getValue() << std::endl
                << "\tli $v0, 11" << std::endl
                << "\tsyscall" << std::endl;
            }
            else if (fe->getType() == st.getPrimitiveType("integer")
                  || fe->getType() == st.getPrimitiveType("boolean"))
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

            if (le->getType() == st.getPrimitiveType("string"))
            {
                std::cout << "\tli $v0, 4" << std::endl;
            }
            else if (le->getType() == st.getPrimitiveType("character"))
            {
                std::cout << "\tli $v0, 11" << std::endl;
            }
            else if (le->getType() == st.getPrimitiveType("integer")
                  || le->getType() == st.getPrimitiveType("boolean"))
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

            if (re->getType() == st.getPrimitiveType("string"))
            {
                std::cout << "\tli $v0, 4" << std::endl;
            }
            else if (re->getType() == st.getPrimitiveType("character"))
            {
                std::cout << "\tli $v0, 11" << std::endl;
            }
            else if (re->getType() == st.getPrimitiveType("integer")
                  || re->getType() == st.getPrimitiveType("boolean"))
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
        auto type = st.getPrimitiveType(i);
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
        //TODO: change the type system.
        auto type = st.getPrimitiveType(i);
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

    /*
    int CodeGenerator::getLvalArr(int lv, int ex)
    {
        
    }
    */

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
        if (le->getType() == st.getPrimitiveType("integer")
         || le->getType() == st.getPrimitiveType("boolean"))
        {
            std::cout << "\tli $v0, 5" << std::endl
            << "\tsyscall" << std::endl
            << "\tsw $v0, " << le->getOffset() << "(" << *le->getRegister() << ")" 
            << std::endl;

        }
        else if (le->getType() == st.getPrimitiveType("character"))
        {
            std::cout << "\tli $v0, 12" << std::endl
            << "\tsyscall" << std::endl
            << "\tsw $v0, " << le->getOffset() << "(" << *le->getRegister() << ")" 
            << std::endl;
        }
        else if (le->getType() == st.getPrimitiveType("string"))
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
    int CodeGenerator::loadReg(LvalExpression* le)
    {
        auto reg = st.requestRegister();

        std::cout
        << "\tlw " << *reg << ", " << le->getOffset() << "(" << *le->getRegister() << ")"
        << std::endl;
        
        auto regExpr = new RegisterExpression();
        regExpr->setRegister(reg);
        regExpr->setType(le->getType());
        expressions.push_back(regExpr);
        return expressions.size() - 1;
    }

    int CodeGenerator::binOp(int l, int r, int (CodeGenerator::*cb)(int, int), bool matchTypes)
    {
        if (matchTypes && (expressions[l]->getType() != expressions[r]->getType()))
        {
            throw std::runtime_error("binary operation on distinct types is prohibited");
        }
        if(dynamic_cast<StringType*>(expressions[l]->getType())
        || dynamic_cast<StringType*>(expressions[r]->getType()))
        {
            throw std::runtime_error("Binary op on string is prohibited");
        }

        if(auto lLvalExpr = dynamic_cast<LvalExpression*>(expressions[l]))
        {
            l = loadReg(lLvalExpr);
        }

        if(auto rLvalExpr = dynamic_cast<LvalExpression*>(expressions[r]))
        {
            r = loadReg(rLvalExpr);
        }

        return (this->*cb)(l,r);
    }
    int CodeGenerator::binOpAdd(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
            std::cout
            << "\tadd " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister()
            << std::endl;

            rRegExpr->releaseRegister();
            return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\taddi " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {
                std::cout
                << "\taddi " << *rRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << ", " << lFExpr->getValue()
                << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() + rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr");
    }

    int CodeGenerator::binOpAnd(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tsne " << *lRegExpr->getRegister() << ", $zero, " << *lRegExpr->getRegister() << std::endl
                << "\tsne " << *rRegExpr->getRegister() << ", $zero, " << *rRegExpr->getRegister() << std::endl
                << "\tand " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << std::endl;

                rRegExpr->releaseRegister();
                lRegExpr->setType(st.getPrimitiveType("boolean"));
                return l;
            }
            else
            if(rFExpr)
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tsne " << *lRegExpr->getRegister() << ", $zero, " << *lRegExpr->getRegister() << std::endl
                << "\tsne " << *reg << ", $zero, " << rFExpr->getValue() << std::endl
                << "\tand " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *reg << std::endl;

                lRegExpr->setType(st.getPrimitiveType("boolean"));
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {
                auto reg = st.requestRegister();
                std::cout
                << "\tsne " << *reg << ", $zero, " << lFExpr->getValue() << std::endl
                << "\tsne " << *rRegExpr->getRegister() << ", $zero, " << *rRegExpr->getRegister() << std::endl
                << "\tand " << *rRegExpr->getRegister() << ", " << *reg << ", " << *rRegExpr->getRegister() << std::endl;

                rRegExpr->setType(st.getPrimitiveType("boolean"));
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() && rFExpr->getValue());
                lFExpr->setType(st.getPrimitiveType("boolean"));
                return l;
            }
        }
        throw std::logic_error("noTypeExpr"); 
    }

    int CodeGenerator::binOpOr(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {

                std::cout
                << "\tsne " << *lRegExpr->getRegister() << ", $zero, " << *lRegExpr->getRegister() << std::endl
                << "\tsne " << *rRegExpr->getRegister() << ", $zero, " << *rRegExpr->getRegister() << std::endl
                << "\tor " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister()
                << std::endl;

                rRegExpr->releaseRegister();
                lRegExpr->setType(st.getPrimitiveType("boolean"));
                return l;
            }
            else
            if(rFExpr)
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tsne " << *lRegExpr->getRegister() << ", $zero, " << *lRegExpr->getRegister() << std::endl
                << "\tsne " << *reg << ", $zero, " << rFExpr->getValue() << std::endl
                << "\tor " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *reg
                << std::endl;

                lRegExpr->setType(st.getPrimitiveType("boolean"));
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tsne " << *reg << ", $zero, " << lFExpr->getValue() << std::endl
                << "\tsne " << *rRegExpr->getRegister() << ", $zero, " << *rRegExpr->getRegister() << std::endl
                << "\tor " << *rRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << ", " << *reg
                << std::endl;

                rRegExpr->setType(st.getPrimitiveType("boolean"));
                return l;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() || rFExpr->getValue());

                lFExpr->setType(st.getPrimitiveType("boolean"));
                return l;
            }
        }
        throw std::logic_error("noTypeExpr"); 
    }

    int CodeGenerator::binOpEq(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tseq " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tli " << *reg << ", " << rFExpr->getValue() << std::endl
                << "\tseq " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *reg << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();

                std::cout
                << "\tli " << *reg << ", " << lFExpr->getValue() << std::endl
                << "\tseq " << *rRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << ", " << *reg << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() == rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr");         
    }

    int CodeGenerator::binOpNeq(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tsne " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\tsne " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();

                std::cout
                << "\tsne " << *rRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << ", " << lFExpr->getValue()
                << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() != rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr"); 
    }
    
    int CodeGenerator::binOpGteq(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tsge " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\tsge " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();

                std::cout
                << "\tli " << *reg << ", " << lFExpr->getValue() << std::endl
                << "\tsge " << *rRegExpr->getRegister() << ", " << *reg << ", " << *rRegExpr->getRegister() << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() >= rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr"); 
    }

    int CodeGenerator::binOpGt(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tsgt " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\tsgt " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();

                std::cout
                << "\tli " << *reg << ", " << lFExpr->getValue() << std::endl
                << "\tsgt " << *rRegExpr->getRegister() << ", " << *reg << ", " << *rRegExpr->getRegister() << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() > rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr");       
    }

    int CodeGenerator::binOpLteq(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tsle " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\tsle " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();

                std::cout
                << "\tli " << *reg << ", " << lFExpr->getValue() << std::endl
                << "\tsle " << *rRegExpr->getRegister() << ", " << *reg << ", " << *rRegExpr->getRegister() << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() <= rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr");
    }

    int CodeGenerator::binOpLt(int l , int r )
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tslt " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister()
                << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\tslti " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();

                std::cout
                << "\tli " << *reg << ", " << lFExpr->getValue() << std::endl
                << "\tslt " << *rRegExpr->getRegister() << ", " << *reg << ", " << *rRegExpr->getRegister() << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() < rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr"); 
    }

    int CodeGenerator::binOpSub(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tsub " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister()
                << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\tsub " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();

                std::cout
                << "\tli " << *reg << ", " << lFExpr->getValue() << std::endl
                << "\tsub " << *rRegExpr->getRegister() << ", " << *reg << ", " << *rRegExpr->getRegister() << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() - rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr"); 
    }
    
    int CodeGenerator::binOpMult(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tmul " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister()
                << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                auto reg = st.requestRegister();

                std::cout
                << "\tmul " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                std::cout
                << "\tmul " << *rRegExpr->getRegister() << ", " << *rRegExpr->getRegister() << ", " << lFExpr->getValue()
                << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() * rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr"); 
    }

    int CodeGenerator::binOpDiv(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\tdiv " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister()
                << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\tdiv " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();
                std::cout
                << "\tli " << *reg << ", " << lFExpr << std::endl
                << "\tdiv " << *rRegExpr->getRegister() << ", " << *reg << ", " << *rRegExpr->getRegister() << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() / rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr");
    }

    int CodeGenerator::binOpMod(int l, int r)
    {
        auto lFExpr = dynamic_cast<FoldExpression*>(expressions[l]);
        auto rFExpr = dynamic_cast<FoldExpression*>(expressions[r]);
        auto lRegExpr = dynamic_cast<RegisterExpression*>(expressions[l]);
        auto rRegExpr = dynamic_cast<RegisterExpression*>(expressions[r]);

        if(lRegExpr)
        {
            if (rRegExpr)
            {
                std::cout
                << "\trem " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << *rRegExpr->getRegister()
                << std::endl;

                rRegExpr->releaseRegister();
                return l;
            }
            else
            if(rFExpr)
            {
                std::cout
                << "\trem " << *lRegExpr->getRegister() << ", " << *lRegExpr->getRegister() << ", " << rFExpr->getValue()
                << std::endl;
                return l;
            }
        }
        else
        if(lFExpr)
        {
            if(rRegExpr)
            {   
                auto reg = st.requestRegister();
                std::cout
                << "\tli " << *reg << ", " << lFExpr << std::endl
                << "\trem " << *rRegExpr->getRegister() << ", " << *reg << ", " << *rRegExpr->getRegister() << std::endl;
                return r;
            }
            else
            if(rFExpr)
            {
                lFExpr->setValue(lFExpr->getValue() % rFExpr->getValue());
                return l;
            }
        }
        throw std::logic_error("noTypeExpr");
    }

    int CodeGenerator::unOp(int i, int (CodeGenerator::*cb)(int))
    {
        if (auto e = dynamic_cast<LvalExpression*>(expressions[i]))
        {
            i = loadReg(e);
        }
        return (this->*cb)(i);
    }

    int CodeGenerator::unOpNeg(int i)
    {
        if (!dynamic_cast<IntegerType *>(expressions[i]->getType()))
        {
            throw std::runtime_error("Non integer negation");
        }
        if (auto e = dynamic_cast<FoldExpression*>(expressions[i]))
        {
            e->setValue(e->getValue() * -1);
            return i;
        }
        else
        if (auto e = dynamic_cast<RegisterExpression*>(expressions[i]))
        {
            auto reg = st.requestRegister();

            std::cout 
            << "\tneg " << *e->getRegister() << ", " << *e->getRegister()
            << std::endl;

            return i;
        }
        throw std::logic_error("noTypeExpr");
    }
    int CodeGenerator::unOpNot(int i)
    {
        if (!dynamic_cast<IntegerType*>(expressions[i]->getType())
         && !dynamic_cast<BooleanType*>(expressions[i]->getType()))
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
            e->setType(st.getPrimitiveType("boolean"));
            return i;
        }
        else

        if (auto e = dynamic_cast<RegisterExpression*>(expressions[i]))
        {
            auto reg = st.requestRegister();
            auto reg2 = st.requestRegister();

            std::cout
            << "\tslti " << *reg << ", " << *e->getRegister() << ", 1" << std::endl
            << "\tslti " << *reg2 << ", " << *e->getRegister() << ", 0" << std::endl
            << "\txor " << *e->getRegister() << ", " << *reg << ", " << *reg2 << std::endl;

            e->setType(st.getPrimitiveType("boolean"));

            return i;
        }
        throw std::logic_error("noTypeExpr");
    }


    int CodeGenerator::unOpDecr(int i)
    {
        if (dynamic_cast<StringType*>(expressions[i]->getType()))
        {
            throw std::runtime_error("Decrement of string datatype is prohibited");
        }

        if (dynamic_cast<BooleanType*>(expressions[i]->getType()))
        {
            return unOpNeg(i);
        }

        if (auto e = dynamic_cast<FoldExpression*>(expressions[i]))
        {
            e->setValue(e->getValue() - 1);
            return i;
        }
        else
        if (auto e = dynamic_cast<RegisterExpression*>(expressions[i]))
        {
            std::cout
            << "\taddi " << *e->getRegister() << ", " << *e->getRegister() << ", -1" << std::endl;

            return i;
        }
        throw std::logic_error("noTypeExpr");
    }

    int CodeGenerator::unOpIncr(int i)
    {
        if (dynamic_cast<StringType*>(expressions[i]->getType()))
        {
            throw std::runtime_error("Increment of string datatype is prohibited");
        }

        if (dynamic_cast<BooleanType*>(expressions[i]->getType()))
        {
            return unOpNeg(i);
        }

        if (auto e = dynamic_cast<FoldExpression*>(expressions[i]))
        {
            e->setValue(e->getValue() + 1);
            return i;
        }
        else
        if (auto e = dynamic_cast<RegisterExpression*>(expressions[i]))
        {
            std::cout
            << "\taddi " << *e->getRegister() << ", " << *e->getRegister() << ", 1" << std::endl;

            return i;
        }
        throw std::logic_error("noTypeExpr");
    }