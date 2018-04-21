#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <map>
#include <memory>
#include "Type.hpp"

struct Function
{
    std::vector<Type*> ineffableTypes;
    std::vector<std::shared_ptr<std::string>> regPool;
    std::vector<int> frameOffsets;
    bool inFunction;
    unsigned offset;
};
#endif