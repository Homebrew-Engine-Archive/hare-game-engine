//***************************************************************
//  File:    D3DVertexBufferManager.cpp
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
#include "D3DVertexBufferManager.h"
#include "D3DRenderSystem.h"



HARE_IMPLEMENT_SINGLETON(D3DVertexBufferManager)

D3DVertexBufferManager::D3DVertexBufferManager(uint32 size, uint32 FVF)
	:bufferSize(size)
	,vertexFVF(FVF)
	,d3dVectexBuffer(NULL)
	,vertexBuffer(NULL)
	,vertexCount(0)
	,primCount(0)
	,isLock(false)
{
    DeviceManager::getSingletonPtr()->registerDeviceObject(this);
    afterResetDevice();
}

D3DVertexBufferManager::~D3DVertexBufferManager()
{
    DeviceManager::getSingletonPtr()->unregisterDeviceObject(this);
	release();
}

void D3DVertexBufferManager::beforeResetDevice()
{
	release();
}

void D3DVertexBufferManager::afterResetDevice()
{
	if (d3dVectexBuffer){
		return ;
	}

	LPDIRECT3DDEVICE9 d3dDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
	assert(d3dDevice);

	if (FAILED(d3dDevice->CreateVertexBuffer(
		sizeof(Vertex) * bufferSize,
		D3DUSAGE_WRITEONLY,
		vertexFVF,
		D3DPOOL_DEFAULT,
		&d3dVectexBuffer,
		NULL))){
		assert(false);
	}

	d3dDevice->SetFVF(D3DRenderSystem::VERTEX_FVF);
	d3dDevice->SetStreamSource(0, d3dVectexBuffer, 0, sizeof(Vertex));
}

void D3DVertexBufferManager::lockBuffer()
{
	if (!isLock){
		d3dVectexBuffer->Lock(0, 0, (void**)&vertexBuffer, 0);
		isLock = true;		
	}
}

void D3DVertexBufferManager::unlockBuffer()
{
	if (isLock){
		d3dVectexBuffer->Unlock();
		isLock = false;		
	}
}

void D3DVertexBufferManager::writeBuffer(Vertex* buffer, uint32 vertexCnt, uint32 primCnt)
{
	assert(isLock);
	memcpy(vertexBuffer + vertexCount, buffer, vertexCnt * sizeof(Vertex));
	vertexCount += vertexCnt;
	primCount += primCnt;
}

//void D3DVertexBufferManager::readBuffer(Vertex* buffer, uint32 size)
//{
//	assert(isLock);
//	memcpy(buffer, vertexBuffer, size);
//}

uint32 D3DVertexBufferManager::getVectexCount()
{
	return vertexCount;
}

uint32 D3DVertexBufferManager::getPrimCount()
{
	return primCount;
}

void D3DVertexBufferManager::release()
{
	SAFE_RELEASE(d3dVectexBuffer);
	vertexCount = 0;
	primCount = 0;
	vertexBuffer = NULL;
	isLock = false;
	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
	if (pD3DDevice){
		pD3DDevice->SetStreamSource(0, NULL, 0, sizeof(Vertex));
	}
}







