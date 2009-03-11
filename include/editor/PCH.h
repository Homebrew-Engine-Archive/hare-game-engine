//***************************************************************
//  File:    PCH.h
//  Date:    10/27/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if HARE_COMPILER == HARE_COMPILER_MSVC
// disable: "<type> needs to have dll-interface to be used by clients'
#pragma warning(disable : 4251)

#endif