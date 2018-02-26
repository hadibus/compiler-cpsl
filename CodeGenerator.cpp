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

    Value CodeGenerator::stringLiteral(char* cstr)
    {
        Value val;
        val.value = st.storeStringLiteral(cstr);
        val.type = st.getPrimativeType("string");
        return val;
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