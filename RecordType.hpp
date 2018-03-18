#ifndef RECORDTYPE_HPP
#define RECORDTYPE_HPP

#include <map>
#include <string>
#include <stdexcept>
#include "Type.hpp"

class RecordType : public Type
{
public:
    RecordType():Type(),members(){};
    RecordType(unsigned s):Type(s), members(){};
    //int getBaseSizeRecursive();
    std::map<std::string,Type*> getMembers(){return members;};
    void addMembers(std::map<std::string,Type*> ms)
    {
        for(const auto & item : ms)
        {
            addMember(item.first,item.second);
        }
    }
    void addMember(std::string s,Type* t)
    {
        auto f = members.find(s);
        if (f == members.end())
        {
           throw std::runtime_error(
               "id " + s + " already defined in record");
        }
        members[s] = t;
    }
    int getSizeRecursive()
    {
        unsigned int totSize = 0U;
        for (auto i = members.begin(); i != members.end(); i++)
        {
            totSize += i->second->getSizeRecursive();
        }
    }
    int size(){return members.size();};

private:
    std::map<std::string,Type*> members;
};

#endif