#include "Frame.hpp"

Frame::Frame(): 
    constants(), variables(), types(), forVars(), regPool(), offset(0)
{
    init();
}

void Frame::init()
{
    const int NUM_T_REGS = 10; // 0-9
    for(auto i = 0; i < NUM_T_REGS; i++)
    {
        regPool.push_back(std::make_shared<std::string>("$t" + std::to_string(i)));
    }

    const int NUM_S_REGS = 8; // 0-7
    for(auto i = 0; i < NUM_S_REGS; i++)
    {
        regPool.push_back(std::make_shared<std::string>("$s" + std::to_string(i)));
    }
}