//***************************************************************
//  File:    Platforms.h
//  Date:    09/29/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _PLATFORMS_H__
#define _PLATFORMS_H__

#define HARE_PLATFORM_WIN32     1
#define HARE_PLATFORM_LINUX     2
#define HARE_PLATFORM_APPLE     3

#define HARE_COMPILER_MSVC      1
#define HARE_COMPILER_GNUC      2
#define HARE_COMPILER_BORL      3

/* Determine the current platform
 */
#if defined(__WIN32__) || defined(_WIN32)
#   define HARE_PLATFORM HARE_PLATFORM_WIN32
#elif defined(__APPLE_CC__)
#   define HARE_PLATFORM HARE_PLATFORM_APPLE
#else
#   define HARE_PLATFORM HARE_PLATFORM_LINUX
#endif

/* Determine the current architecture
 */
#if defined(__x86_64__) || defined(_M_X64) || \
    defined(__powerpc64__) || defined(__alpha__) || \
    defined(__ia64__) || defined(__s390__) || \
    defined(__s390x__)
#   pragma error "currnetly only support architecture 32 !"
#endif

/* Determine the current complier
 */
#if defined(_MSC_VER)
#   define HARE_COMPILER HARE_COMPILER_MSVC
#   define HARE_COMP_VER _MSC_VER
#elif defined(__GNUC__)
#   define HARE_COMPILER HARE_COMPILER_GNUC
#   define HARE_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
#elif defined( __BORLANDC__ )
#   define HARE_COMPILER HARE_COMPILER_BORL
#   define HARE_COMP_VER __BCPLUSPLUS__
#else
#   pragma error "unknown compiler !"
#endif

#if HARE_COMPILER == HARE_COMPILER_GNUC && HARE_COMP_VER >= 310 && !defined(STLPORT)
#   define HashMap ::__gnu_cxx::hash_map
#else
#   if HARE_COMPILER == HARE_COMPILER_MSVC
#       if HARE_COMP_VER > 1300 && !defined(_STLP_MSVC)
#           define HashMap ::stdext::hash_map
#       else
#           define HashMap ::std::hash_map
#       endif
#   else
#       define HashMap ::std::hash_map
#   endif
#endif

#if HARE_COMPILER == HARE_COMPILER_MSVC
// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning(disable : 4251)
// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#   pragma warning(disable : 4661)
// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning(disable : 4275)
#endif

#endif
