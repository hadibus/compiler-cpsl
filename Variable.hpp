#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>

#include "Type.hpp"

struct Variable
{
    Variable() : type(nullptr), offset(0U), reg("$gp"){};
    Type * type;
    unsigned offset;
    std::string reg;
};

#endif