#pragma once

//--------------------------------------------------------------------------------------
// Platform specific include files and defines
//--------------------------------------------------------------------------------------
#ifdef _WIN32
#undef UNICODE

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>
#include <mmsystem.h>
#include <aclapi.h>
#endif

#ifdef __cplusplus
#include <iostream>
#include <sstream>
#include <memory>
#ifndef __ANDROID__	/* Skip these on android devices */
/*#include <thread>
#include <mutex>
#include <atomic>*/
#endif
#include <fstream>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>

#ifdef _WIN32
#include <conio.h>
#include <process.h>
#else
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#endif


#ifdef __APPLE__
#include <mach/mach_types.h>
#include <mach/mach_time.h>
#endif


/*
 * Determine if this is a mobile platform
 */

#ifdef __APPLE__
 #include "TargetConditionals.h"
 #if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
  #define __MOBILE_OS__
 #else
  #include "ke_osx_util.h"
 #endif
#endif

#ifdef _WIN32
#include "ke_win32_util.h"
#endif

#ifdef __ANDROID__
#define __MOBILE_OS__
#endif

#ifdef __QNX__ /* Blackberry */
#define __MOBILE_OS__
#endif

#define Yes	1
#define No	0

/* For non-Windows compilers */
#ifndef PURE
#define PURE =0
#endif

#ifndef BOOL
#define BOOL int
//#define TRUE Yes
//#define FALSE No
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef _WIN32
typedef struct _RECT
{
    long left;
    long top;
    long right;
    long bottom;
}RECT;

#define ZeroMemory(a,b) memset( a, 0, b )
#endif
