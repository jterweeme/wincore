#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __GNUC__
#define PACKED __attribute__ ((packed))
#else
#define PACKED
#endif

#if 0
#include "mystl.h"
#include "heapsort.h"
using namespace mystl;
#else
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


