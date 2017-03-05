#pragma once

//--------------------------------------------------------------------------------------
// Platform specific include files and defines
//--------------------------------------------------------------------------------------
#ifdef _WIN32
#undef UNICODE

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <aclapi.h>
#endif

#ifdef __cplusplus
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
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
#include <direct.h>
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
  #include "KeiOSUtil.h"
 #else
  #include "KeOSXUtil.h"
 #endif
#endif

/* Windows phone or Surface */
#ifdef _WIN32
#if ( WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP )
#define __MOBILE_OS__
#endif
#endif

#ifdef _WIN32
#include "KeWin32Util.h"
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

#if !defined( _WIN32 )
typedef struct _RECT
{
    long left;
    long top;
    long right;
    long bottom;
}RECT;

#define ZeroMemory(a,b) memset( a, 0, b )
#endif

#ifndef _INC_MMREG	// mmreg.h
/* Ripped this from PC headers */
typedef struct {
    uint16_t  wFormatTag;
    uint16_t  nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t  nBlockAlign;
} WAVEFORMAT;

typedef struct {
    uint16_t  wFormatTag;
    uint16_t  nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t  nBlockAlign;
    uint16_t  wBitsPerSample;
    uint16_t  cbSize;
} WAVEFORMATEX;
#endif

#ifndef mmioFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |   \
((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24 ))
#define mmioFOURCC MAKEFOURCC
#endif
