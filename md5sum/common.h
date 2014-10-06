#ifndef _COMMON_H_
#define _COMMON_H_
#include "mystl.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <endian.h>
#include <sstream>
#include <vector>

namespace koe
{
    using std::fstream;
    using std::istream;
    using std::cout;
    using std::string;
    using std::ifstream;
    using std::vector;
    using std::ostringstream;
    using std::ostream;
    using std::ios;
    using std::setw;
    using std::setfill;
    using std::hex;
    using mystl::memcpy;
    using mystl::memset;
}

using namespace koe;

#endif


