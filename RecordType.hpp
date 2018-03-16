#ifndef RECORDTYPE_HPP
#define RECORDTYPE_HPP

#include <map>
#include <string>
#include "Type.hpp"

class RecordType : public Type
{
public:
    RecordType():Type(),members(){};

private:
    std::map<std::string,Type*> members;
};

#endif