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
#include "mystl.h"
#include "heapsort.h"
#else
//#include "mystl.h"
//#include "heapsort.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <bitset>
#include <cstring>
#include <algorithm>
#include <array>
using namespace std;
#endif

#endif


