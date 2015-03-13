#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __GNUC__
#define PACKED __attribute__ ((packed))
#else
#define PACKED
#endif

namespace mystl { }

#if 1
using namespace mystl;
#else
using namespace std;
#endif

#if 1
#include <algorithm>
#include <array>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#endif
#include "mystl.h"

#endif


