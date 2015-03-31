#ifndef _MYTYPES_H_
#define _MYTYPES_H_

class MyTypes
{
public:
    typedef char int8_t;
    typedef unsigned char uint8_t;
    typedef short int16_t;
    typedef unsigned short uint16_t;
    typedef int int32_t;
    typedef unsigned uint32_t;
    typedef long long int64_t;
    typedef unsigned long long uint64_t;
    typedef long unsigned size_t;
    typedef long int ptrdiff_t;
    typedef uint32_t streampos;
    typedef int32_t streamsize;
    typedef int32_t streamoff;
};

namespace mystl
{
    typedef MyTypes::streampos streampos;
    typedef MyTypes::size_t size_t;
#ifndef _STDINT_H
    typedef MyTypes::int8_t int8_t;
    typedef MyTypes::uint8_t uint8_t;
    typedef MyTypes::int16_t int16_t;
    typedef MyTypes::uint16_t uint16_t;
    typedef MyTypes::int32_t int32_t;
    typedef MyTypes::uint32_t uint32_t;
    typedef MyTypes::int64_t int64_t;
    typedef MyTypes::uint64_t uint64_t;
#endif
}


#endif



