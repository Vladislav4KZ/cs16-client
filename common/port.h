/*
port.h -- Portability Layer for Windows types
Copyright (C) 2015 Alibek Omarov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once
#ifndef PORT_H
#define PORT_H

#include "build.h"

#ifdef XASH_VGUI
	#if !(defined(__i386__) || defined(_X86_) || defined(_WIN32))
	#error "VGUI is exists only for x86. You must disable VGUI flag or build Xash3D for x86 target."
    #endif
#endif

#ifndef _WIN32
	#if XASH_LINUX == 1
	#include <limits.h>
	#include <dlfcn.h>
	#endif

    #ifdef __APPLE__
		#include <sys/syslimits.h>
		#define OS_LIB_EXT "dylib"
    #else
		#ifdef __linux__
			#include <linux/limits.h>
		#endif
		#define OS_LIB_EXT "so"
    #endif

    #ifdef __ANDROID__
		#define XASH_THREADS

        #define ANDROID_LIB_PLATFORM "android"

        #if defined(__aarch64__) || defined(_M_ARM64)
            #define ANDROID_LIB_ARCH "arm64"
        #elif defined(__x86_64__) || defined(_M_X64)
            #define ANDROID_LIB_ARCH "amd64"
        #elif defined(__i386__) || defined(_M_IX86)
            #define ANDROID_LIB_ARCH "i386"
        #elif defined(__arm__) || defined(_M_ARM)
            #if defined(__ARM_ARCH_8__) || (defined(__ARM_ARCH) && __ARM_ARCH == 8)
                #define ANDROID_LIB_CPU "armv8_32"
            #elif defined(__ARM_ARCH_7__) || (defined(__ARM_ARCH) && __ARM_ARCH == 7)
                #define ANDROID_LIB_CPU "armv7"
            #elif defined(__ARM_ARCH_6__) || (defined(__ARM_ARCH) && __ARM_ARCH == 6)
                #define ANDROID_LIB_CPU "armv6"
            #elif defined(__ARM_ARCH_5__) || (defined(__ARM_ARCH) && __ARM_ARCH == 5)
                #define ANDROID_LIB_CPU "armv5"
            #elif defined(__ARM_ARCH_4__) || (defined(__ARM_ARCH) && __ARM_ARCH == 4)
                #define ANDROID_LIB_CPU "armv4"
            #else
                #define ANDROID_LIB_CPU "arm"
            #endif

            #if defined(__SOFTFP__) || (defined(__ARM_PCS_VFP) && __ARM_PCS_VFP == 0)
                #define ANDROID_LIB_FPU "l"
            #else
                #define ANDROID_LIB_FPU "hf"
            #endif

            #define ANDROID_LIB_ARCH ANDROID_LIB_CPU ANDROID_LIB_FPU
        #endif

        #ifdef ANDROID_LIB_ARCH
            #define ANDROID_LIB_POSTFIX "_" ANDROID_LIB_PLATFORM "_" ANDROID_LIB_ARCH
        #else
            #define ANDROID_LIB_POSTFIX "_" ANDROID_LIB_PLATFORM
        #endif

        #define MENUDLL "libmenu" ANDROID_LIB_POSTFIX "." OS_LIB_EXT
        #define CLIENTDLL "libclient" ANDROID_LIB_POSTFIX "." OS_LIB_EXT
        #define SERVERDLL "libserver" ANDROID_LIB_POSTFIX "." OS_LIB_EXT
        #define GAMEPATH "/sdcard/xash"
    #else
        #define MENUDLL "libxashmenu." OS_LIB_EXT
        #define CLIENTDLL "client." OS_LIB_EXT
        #ifdef PANDORA
            #define SERVERDLL "hl." OS_LIB_EXT
            #define LIBPATH "."
            #define GAMEPATH "."
        #endif
    #endif

	#define VGUI_SUPPORT_DLL "libvgui_support." OS_LIB_EXT

	#define TRUE	    1
	#define FALSE	    0

    // Windows-specific
    #define _stdcall
    #define __stdcall
    #define __cdecl
	#define _inline	    static inline
    #define O_BINARY    0		//In Linux O_BINARY didn't exist

    // Windows functions to Linux equivalent
	#define _mkdir( x )					mkdir( x, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )
	#define LoadLibrary( x )			dlopen( x, RTLD_NOW )
	#define GetProcAddress( x, y )		dlsym( x, y )
	#define SetCurrentDirectory( x )	(!chdir( x ))
	#define FreeLibrary( x )			dlclose( x )
	#define MAKEWORD(a,b)				((short int)(((unsigned char)(a))|(((short int)((unsigned char)(b)))<<8)))
	#define max(a, b)  (((a) > (b)) ? (a) : (b))
	#define min(a, b)  (((a) < (b)) ? (a) : (b))
	#define tell(a)						lseek(a, 0, SEEK_CUR)

    typedef unsigned char   BYTE;
    typedef unsigned char   byte;
    typedef short int	    WORD;
    typedef unsigned int    DWORD;
    typedef long int	    LONG;
    typedef unsigned long int   ULONG;
    typedef long	    WPARAM;
	typedef unsigned int    LPARAM;

    typedef void* HANDLE;
    typedef void* HMODULE;
    typedef void* HINSTANCE;

    typedef char* LPSTR;

    typedef struct tagPOINT
    {
	int x, y;
    } POINT;
#else
	#define strcasecmp _stricmp
	#define strncasecmp _strnicmp
	#define open _open
	#define read _read

	// shut-up compiler warnings
	#pragma warning(disable : 4244)	// MIPS
	#pragma warning(disable : 4018)	// signed/unsigned mismatch
	#pragma warning(disable : 4305)	// truncation from const double to float
	#pragma warning(disable : 4115)	// named type definition in parentheses
	#pragma warning(disable : 4100)	// unreferenced formal parameter
	#pragma warning(disable : 4127)	// conditional expression is constant
	#pragma warning(disable : 4057)	// differs in indirection to slightly different base types
	#pragma warning(disable : 4201)	// nonstandard extension used
	#pragma warning(disable : 4706)	// assignment within conditional expression
	#pragma warning(disable : 4054)	// type cast' : from function pointer
	#pragma warning(disable : 4310)	// cast truncates constant value
	#pragma warning(disable : 4244) // 'argument': conversion from 'float' to 'int', possible loss of data

	#define HSPRITE WINAPI_HSPRITE
	#include <windows.h>
	#undef HSPRITE

    #define OS_LIB_EXT "dll"
    #define MENUDLL "menu." OS_LIB_EXT
    #define CLIENTDLL "client." OS_LIB_EXT
	#define VGUI_SUPPORT_DLL "../vgui_support." OS_LIB_EXT
#endif

#endif
