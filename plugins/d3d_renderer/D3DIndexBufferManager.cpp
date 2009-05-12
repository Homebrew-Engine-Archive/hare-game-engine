//***************************************************************
//  File:    D3DIndexBufferManager.cpp
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
#include "D3DIndexBufferManager.h"
#include "D3DRenderSystem.h"



HARE_IMPLEMENT_SINGLETON(D3DIndexBufferManager)

D3DIndexBufferManager::D3DIndexBufferManager(uint32 size)
	:IndexBufferSize(size)
	,d3dIndexBuffer(NULL)
{
    DeviceManager::getSingletonPtr()->registerDeviceObject(this);
	afterResetDevice();
}

D3DIndexBufferManager::~D3DIndexBufferManager()
{
    DeviceManager::getSingletonPtr()->unregisterDeviceObject(this);
	release();
}

void D3DIndexBufferManager::beforeResetDevice()
{
	release();
}

void D3DIndexBufferManager::afterResetDevice()
{
	if (d3dIndexBuffer){
		return ;
	}

	LPDIRECT3DDEVICE9 d3dDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
	assert(d3dDevice);

	if (FAILED(d3dDevice->CreateIndexBuffer(
		IndexBufferSize * 3, // 
		D3DUSAGE_WRITEONLY, 
		D3DFMT_INDEX16, 
		D3DPOOL_DEFAULT, 
		&d3dIndexBuffer,
		NULL))){
		assert(false);
	}

	WORD *pIndices, n = 0;

	if (FAILED(d3dIndexBuffer->Lock(0, 0, (void**)&pIndices, 0))){
		assert(false);
	}

	for(uint32 i = 0; i < IndexBufferSize / 4; i++)
	{
		*pIndices++ = n;
		*pIndices++ = n + 1;
		*pIndices++ = n + 2;
		*pIndices++ = n + 2;
		*pIndices++ = n + 3;
		*pIndices++ = n;
		n += 4;
	}

	d3dIndexBuffer->Unlock();
	d3dDevice->SetIndices(d3dIndexBuffer);

}

void D3DIndexBufferManager::release()
{
	SAFE_RELEASE(d3dIndexBuffer);
	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
	pD3DDevice->SetIndices(NULL);
}
