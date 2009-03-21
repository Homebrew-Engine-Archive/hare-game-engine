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

#include "core/Core.h"
#include "editor/Editor.h"
using namespace hare_editor;

#include "../../bindings/lua/LuaDebugData.h"
#include "../../bindings/lua/LuaDebugDefines.h"
#include "../../bindings/lua/SocketHelper.h"