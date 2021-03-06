#ifndef TYPE_HPP
#define TYPE_HPP

class Type
{
public:
    Type(): siz(0U){};
    Type(unsigned size): siz(size){};
    virtual ~Type(){};
    unsigned size(){return siz;};
    virtual int getSizeRecursive() = 0;
private:
    unsigned siz;
};

#endif