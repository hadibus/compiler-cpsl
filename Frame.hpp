#ifndef FRAME_HPP
#define FRAME_HPP

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "Constant.hpp"
#include "Variable.hpp"

struct Frame
{
    Frame();
    void init();
    std::map<std::string, Constant> constants;
    std::map<std::string, Variable> variables;
    std::map<std::string, Type *> types;
    std::vector<std::pair<std::string, Variable>> forVars;
    std::vector<std::shared_ptr<std::string>> regPool;
    int offset;
};

#endif