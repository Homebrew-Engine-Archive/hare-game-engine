#ifndef _STD_HEADERS_H_
#define _STD_HEADERS_H_

#include "Platform.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>

// Note - not in the original STL, but exists in SGI STL and STLport
#if (HARE_COMPILER == HARE_COMPILER_GNUC) && !defined(STLPORT)
#   include <ext/hash_map>
#   include <ext/hash_set>
#else
#   include <hash_set>
#   include <hash_map>
#endif

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <strstream>

#endif
