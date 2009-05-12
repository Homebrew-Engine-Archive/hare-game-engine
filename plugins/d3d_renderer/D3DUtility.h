//***************************************************************
//  File:    D3DUtility.h
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
#ifndef _D3DUTILITY_H_
#define _D3DUTILITY_H_


class D3DUtility
{
public:
	static void downLoadSurface(LPDIRECT3DSURFACE9 d3dSur, HarePixelFormat format, Image& desImg, const RectN& rc = RectN(0,0,0,0));
};


#endif