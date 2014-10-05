/*
Jasper ter Weeme
*/

#ifndef _COMMON_H_
#define _COMMON_H_
#include "mystl.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

namespace koe
{
    using mystl::istream;
    using mystl::cin;
    using mystl::ifstream;
    using mystl::ostream;
    using mystl::cout;
    using mystl::hex;
    using mystl::oct;
    using mystl::setw;
    using mystl::setfill;
    using mystl::uint8_t;
    using mystl::uint16_t;
};

#endif



