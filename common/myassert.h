#ifndef _MYASSERT_H_
#define _MYASSERT_H_

class MyAssert
{
public:
    void equals(uint32_t a, uint32_t b) { if (a != b) throw "error"; }
};

#endif


