#ifndef default_includes_h
#define default_includes_h


// we have a problem with debug builds: STL list and map change it's sizes
// between release and debug versions. since we have the release version of
// bf2, it's STL's containers are of the corresponding size. if we compiled
// in the debug mode we couldn't access bf's list of players (for example)
// through STL standard "api"... the fix that i've found is to don't use the
// _DEBUG declare and the c++ debug libraries (but can compile with debug
// simbols though).
#ifndef NDEBUG
#undef _DEBUG
#define NDEBUG
#endif

#define _WIN32_WINNT 0x0501
#define WINVER 0x0501
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>


#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <algorithm>
using namespace std;


#include "configuration_switches.h"

#endif //default_includes_h