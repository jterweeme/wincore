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
    using std::ostream;
    using std::cout;
    using std::hex;
    using std::oct;
    using std::setw;
    using std::setfill;
    using mystl::uint8_t;
    using mystl::uint16_t;
};

#endif



