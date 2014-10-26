#ifndef _COMMON_H_
#define _COMMON_H_

#if 1
#include "mystl.h"

namespace koe
{
    using mystl::uint8_t;
    using mystl::uint32_t;
    //using std::fstream;
    using mystl::istream;
    using mystl::cout;
    using mystl::cin;
    using mystl::cerr;
    using mystl::string;
    using mystl::ifstream;
    //using std::ostringstream;
    using mystl::ostream;
    //using std::ios;
    using mystl::setw;
    using mystl::setfill;
    using mystl::hex;
    //using mystl::memcpy;
    using mystl::memset;
    //using mystl::stoul;
}

using namespace mystl;
#else
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <iomanip>
//#include <endian.h>
#include <string.h>
#include <stdlib.h>
#include <string>
using namespace std;
#endif

#endif


