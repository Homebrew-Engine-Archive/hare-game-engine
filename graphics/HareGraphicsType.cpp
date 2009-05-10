//***************************************************************
//  File:    HareGraphicsType.cpp
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "HareGraphicsType.h"

namespace hare
{
	HARE_IMPLEMENT_DYNAMIC_CLASS(Color, Object, 0)
	{
		HARE_META(R, float)
		HARE_META(G, float)
		HARE_META(B, float)
		HARE_META(A, float)
	}
}
