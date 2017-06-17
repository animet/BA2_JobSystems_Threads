// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <functional>
#include <vector>
#include <queue>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <assert.h>
#include <chrono>
#include <atomic>
#include <string.h>
#include <conio.h>

#include <io.h> //for _open function (MSDN)
#include <fcntl.h> //for _open function (MSDN)
#include <sys\types.h> //for _open function (MSDN)
#include <sys\stat.h> //for _open function (MSDN)
#include <direct.h> //for mkdir function (MSDN)

#include "Windows.h" //For fiber/UMS functions

// TODO: reference additional headers your program requires here
#include "UserLevelImplementation.h"

//#define JOBSYSTEM_DEBUG_LOG 1
#define USE_STEPS_FOR_MEMORY_ANALYSIS 1