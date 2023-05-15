#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN          
#include <windows.h>

#include <GdiPlus.h>
#pragma comment(lib,"gdiplus")
using namespace Gdiplus;

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")

#include <io.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <queue>

using namespace std;


// My Header
#include "Define.h"
#include "Enum.h"
#include "EnumString.h"
#include "Macro.h"
#include "Struct.h"
#include "Template.h"
#include "Global.h"