#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __GNUC__
#define PACKED __attribute__ ((packed))
#else
#define PACKED
#endif

namespace mystl { }

#ifndef NO_MYSTL
using namespace mystl;
#include "mystl.h"
#else
using namespace std;
#include <algorithm>
#include <array>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <regex>
#endif

#endif


