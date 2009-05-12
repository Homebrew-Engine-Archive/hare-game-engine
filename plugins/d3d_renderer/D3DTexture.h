//***************************************************************
//  File:    D3DTexture.h
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
#ifndef _D3DTEXTURE_H_
#define _D3DTEXTURE_H_
#include "DeviceManager.h"


class D3DTexture : public Texture, public DeviceObject
{
public:
	D3DTexture();

	virtual ~D3DTexture();

	virtual void active();

	virtual void inactive();

	virtual void beforeResetDevice();

	virtual void afterResetDevice();

	virtual void upload(const Image& img, uint32 destX = 0, uint32 destY = 0);

	virtual void download(Image& img, const RectN& rc = RectN(0,0,0,0));

	virtual void reCreate();

	virtual void release();

	LPDIRECT3DTEXTURE9 getD3DTexture();

protected:
	bool doCreate();
    void doRelease();

private:
	LPDIRECT3DTEXTURE9 d3dTexture;

	LPDIRECT3DDEVICE9  pD3DDevice;

	LPDIRECT3DSURFACE9 pDepthStencilSurface; //渲染到纹理用

	D3DXMATRIX MatProj; //渲染到纹理用
	D3DXMATRIX MatView; //渲染到纹理用
};


#endif