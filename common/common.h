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

#include <algorithm>
#include <array>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "mystl.h"
#include "heapsort.h"

#endif


