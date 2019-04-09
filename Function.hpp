#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <map>
#include <memory>
#include "Type.hpp"
#include "Variable.hpp"


struct Function
{
    Function(): offset(0){init();};
    int offset;
    std::map<std::string, Variable> parameters;
    std::vector<std::string> paramNames;
    std::vector<Type*> ineffableTypes;
    std::vector<std::shared_ptr<std::string>> regPool;
    std::vector<int> frameOffsets;
private:
    void init()
    {
        const unsigned NUM_TS = 10, NUM_SS = 8;
        for (auto i = 0; i < NUM_TS; i++)
        {
            regPool.push_back(std::make_shared<std::string>("$t" + std::to_string(i)));
        }
        for (auto i = 0; i < NUM_SS; i++)
        {
            regPool.push_back(std::make_shared<std::string>("$s" + std::to_string(i)));
        }
    };
};
#endif