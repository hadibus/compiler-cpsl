#ifndef CHARACTERTYPE_HPP
#define CHARACTERTYPE_HPP

#include "Primitive.hpp"

class CharacterType : public Primitive
{
public:
    CharacterType():Primitive(4U){};
    virtual ~CharacterType(){};
};

#endif