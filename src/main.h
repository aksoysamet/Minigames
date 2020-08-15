#ifndef MAIN_H_INCLUDED

#define MAIN_H_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
#include "windows.h"
#else
//#include "pthread.h"
#include <unistd.h>
#endif
#include "rxml/rapidxml.hpp"
#include "rxml/rapidxml_utils.hpp"
#include "SDK/plugin.h"
#include "sampgdk.h"
#include "gdk_streamer.h"
#define PLUGIN_FUNCTION cell AMX_NATIVE_CALL
using namespace std;
#endif // MAİN_H_INCLUDED