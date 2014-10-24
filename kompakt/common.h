#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __GNUC__
#define PACKED __attribute__ ((packed))
#else
#define PACKED
#endif

#if 1
#include "mystl.h"
using namespace mystl;
#else
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <bitset>
#include <string.h>
#include <algorithm>
using namespace std;
#endif

#endif


