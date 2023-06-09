//***************************************************************
//  File:    D3DTexture.cpp
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
#include "D3DTexture.h"
#include "D3DRenderSystem.h"
#include "D3DTypeConverter.h"
#include "D3DRenderWindow.h"
#include "D3DUtility.h"



D3DTexture::D3DTexture()
	:d3dTexture(NULL)
	,pDepthStencilSurface(NULL)
{
	pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
	assert(pD3DDevice);
}

D3DTexture::~D3DTexture()
{
	release();
}

LPDIRECT3DTEXTURE9 D3DTexture::getD3DTexture()
{
	return d3dTexture;
}

void D3DTexture::active()
{
	assert(bIsRenderable);
	assert(pD3DDevice);

	D3DRenderSystem::getSingletonPtr()->beginFrame();

	HRESULT hr;

	LPDIRECT3DSURFACE9 sur;
	hr = d3dTexture->GetSurfaceLevel(0, &sur);
	if (FAILED(hr))
		assert(false);

	hr = pD3DDevice->SetRenderTarget(0, sur); 
	if (FAILED(hr))
		assert(false);

	//没有Z pDepthStencilSurface为空
	hr = pD3DDevice->SetDepthStencilSurface(pDepthStencilSurface);
	
	if (FAILED(hr))
		assert(false);

	SAFE_RELEASE(sur);

    D3DRenderSystem::getSingletonPtr()->clear(pDepthStencilSurface != NULL);

    RenderSystem::getSingletonPtr()->setProjection(0, projectionWidth, 0, projectionHeight);

}

void D3DTexture::inactive()
{
	assert(bIsRenderable);

	D3DRenderSystem::getSingletonPtr()->render();

	D3DRenderSystem::getSingletonPtr()->endFrame();
}

void D3DTexture::beforeResetDevice()
{
	assert(bIsRenderable);
	doRelease();

}

void D3DTexture::afterResetDevice()
{
	assert(bIsRenderable);
    bool ok = doCreate();
	assert(ok);
}

void D3DTexture::upload(const Image &img, uint32 destX, uint32 destY)
{
	assert(img.getImageData() != NULL && destX >= 0 && destY >= 0 && destX < width && destY < height);

	int desWidth = min(img.getWidth(), width - destX);
	int desHeight= min(img.getHeight(), height- destY);

	RECT srcRect = { 0, 0, img.getWidth(), img.getHeight() };
	RECT desRect = { destX, destY, destX + desWidth, destY + desHeight };

	HRESULT hr;
	LPDIRECT3DSURFACE9 sur;
	hr = d3dTexture->GetSurfaceLevel(0, &sur);
	if (FAILED(hr))
		assert(false);

	hr = D3DXLoadSurfaceFromMemory(
		sur, 
		NULL, 
		&desRect, 
		img.getImageData(), 
		D3DTypeConverter::toD3DFormat(img.getPixelFormat()), 
		img.getRowStride(), 
		NULL, 
		&srcRect, 
		D3DX_FILTER_POINT, 
		0);

	if (FAILED(hr))
		assert(false);

	SAFE_RELEASE(sur);

}

void D3DTexture::download(Image &img, const RectN& rc)
{
	HRESULT hr;
	LPDIRECT3DSURFACE9 destSurface;
	hr = d3dTexture->GetSurfaceLevel(0,&destSurface);
	D3DUtility::downLoadSurface(destSurface, texPixelFormat, img, rc);
	SAFE_RELEASE(destSurface);
}

void D3DTexture::recreate()
{
	//注册自己到设备对象管理
	if (bIsRenderable){
		DeviceManager::getSingletonPtr()->registerDeviceObject(this);
	}

    bool ok = doCreate();
	assert(ok);
}

void D3DTexture::release()
{
	//移除自己到设备对象管理
	if (bIsRenderable){
		DeviceManager::getSingletonPtr()->unregisterDeviceObject(this);
	}
	
	doRelease();
}

bool D3DTexture::doCreate()
{

	if (!bFromImage){
		if (D3DTypeConverter::toD3DFormat(texPixelFormat) == D3DFMT_UNKNOWN)
			return false;

		if (FAILED(pD3DDevice->CreateTexture(//此函数必须调整width，height为2的幂次方否则创建失败 在调用createRenderToTex函数时长宽已经被调整过了
			width,
			height,
			1,						//minimap级别1
			bIsRenderable ? D3DUSAGE_RENDERTARGET : 0,	//不能用D3DUSAGE_DYNAMIC 它需要显卡支持，D3DUSAGE_DYNAMIC说明可以lock但是再用D3DPOOL_DEFAULT说明纹理会在agp中创建
			D3DTypeConverter::toD3DFormat(texPixelFormat),		//我们的格式
			bIsRenderable ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED,
			&d3dTexture,
			NULL))){
			return false;
		}

		if (bIsRenderable){
            const D3DMULTISAMPLE_TYPE MultiSample = D3DMULTISAMPLE_NONE;
            const DWORD MultisampleQuality = 0;
            const bool Discard = TRUE;

			if (FAILED(pD3DDevice->CreateDepthStencilSurface(
				width,
				height,
				D3DFMT_D16,
				MultiSample,
				MultisampleQuality,
				Discard,
				&pDepthStencilSurface,
				NULL
				))){
					return false;
				}					
		}

	}else{
		Image img;
		assert(!fileName.empty());
		if (!img.loadFromFile(fileName))
            return false;

		uint32 srcWidth = img.getWidth();
		uint32 srcHeight= img.getHeight();

		uint32 PO2Width, PO2Height;
		if (!MathUtil::isPO2(srcWidth) || !MathUtil::isPO2(srcHeight)) {

			PO2Width  = MathUtil::firstPO2From(srcWidth);
			PO2Height = MathUtil::firstPO2From(srcHeight);
			if (PO2Width == 0)
				PO2Width = 1;	

			if (PO2Height == 0)
				PO2Height = 1;

		}else {
			PO2Width  = srcWidth;
			PO2Height = srcHeight;
		}

		width = PO2Width;
		height= PO2Height;
			
		if (FAILED(pD3DDevice->CreateTexture( //此函数必须调整width，height为2的幂次方否则创建失败
			width,
			height,
			1,						//minimap级别1
			0,
			D3DFMT_A8R8G8B8,		//我们的格式
			D3DPOOL_MANAGED,        //由系统管理内存 设备丢失时不必重建
			&d3dTexture,
			NULL))){
			return false;
		}

		//以后为image添加函数可以从一种格式的图片转换为另一种格式的图片例如 rgb 转为 argb
		int PixelFB = GraphicsUtil::getPixelFormatBytes(img.getPixelFormat());
		assert(PixelFB == 4);

		D3DLOCKED_RECT d3dlr;//D3DPOOL_MANAGED系统内存池可以lock

		d3dTexture->LockRect(0, &d3dlr, 0, 0);
	
		uint8 *pDestBuf = (uint8*)d3dlr.pBits;

		uint8 *pSrcBuf  = (uint8*)img.getImageData();

		//swap R and B 
		if (img.getPixelFormat() == HPF_A8B8G8R8){
			HarePixelType<HPF_A8B8G8R8>* tmpData = (HarePixelType<HPF_A8B8G8R8>*)pSrcBuf;
			for (uint32 count = 0; count < img.getWidth() * img.getHeight(); ++count){
				uint8 tmp;
				tmp        = tmpData->r;
				tmpData->r = tmpData->b;
				tmpData->b = tmp;
				tmpData++;
			}
		}

        texPixelFormat = HPF_A8R8G8B8;

		for (uint32 y = 0; y < srcHeight; ++y){
			memcpy(pDestBuf + y * PO2Width * PixelFB, pSrcBuf + y * srcWidth * PixelFB, srcWidth * PixelFB);
		}

		d3dTexture->UnlockRect(0);

	}

	return true;
}

void D3DTexture::doRelease()
{
	if (bIsRenderable){	
		SAFE_RELEASE(pDepthStencilSurface)	
	}

	SAFE_RELEASE(d3dTexture)
}









