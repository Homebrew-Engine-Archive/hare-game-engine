//***************************************************************
//  File:    D3DRenderSystem.cpp
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
#include "D3DIndexBufferManager.h"
#include "D3DRenderWindow.h"
#include "D3DRenderSystem.h"
#include "D3DTexture.h"
#include "D3DTypeConverter.h"


const uint32 D3DRenderSystem::VERTEX_CAPACITY = 1024;

const uint32 D3DRenderSystem::VERTEX_FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);

D3DRenderSystem::D3DRenderSystem()
	:pD3DDevice(NULL)
	,pD3DInterface(NULL)
	,bAnisotropicFilter(false)
	,PrimType(D3DPT_TRIANGLELIST)
	,curRenderTexture(NULL)
	,pIndexBufferManager(NULL)
	,pVertexBufferManager(NULL)
	,pPrimaryWindow(NULL)
	,texMat(Matrix4::IDENTITY)
    ,bUseIndex(true)
{
	pD3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3DInterface) {
		assert(false);
	}
}

D3DRenderSystem::~D3DRenderSystem()
{
	
}

void D3DRenderSystem::initalize()
{
	HRESULT hr;

	hr = pD3DInterface->GetDeviceCaps(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		&d3dCaps);
	if (hr == D3DERR_NOTAVAILABLE){
		//3D硬件加速不可用
	}else if (FAILED(hr)){
		//无法恢复d3d Device Caps
	}

	pDeviveManager = new DeviceManager;

}

void D3DRenderSystem::release()
{
	if (pVertexBufferManager){
		pVertexBufferManager->release();
		delete pVertexBufferManager;
		pVertexBufferManager = NULL;
	}

	if (pIndexBufferManager){
		pIndexBufferManager->release();
		delete pIndexBufferManager;
		pIndexBufferManager = NULL;		
	}

	SAFE_DELETE(pDeviveManager)
	SAFE_RELEASE(pD3DDevice)
	SAFE_RELEASE(pD3DInterface)
}

void D3DRenderSystem::initalizeParam()
{
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	setShaderParams(curShaderParams);
	setTextureStage(curTextureStage);
	pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 ); 
	texMat = Matrix4::IDENTITY;
	pD3DDevice->SetTransform(D3DTS_TEXTURE0, &D3DTypeConverter::toD3DMatrix(texMat));

}

void D3DRenderSystem::createDevice(D3DRenderWindow* renderWindow)
{
	HRESULT hr;
	if (renderWindow->getIsMainWnd()){

		if (pD3DDevice){//D3D设备已经存在 调用原因可能是设备重建
			initalizeParam();
			
			hr = pD3DDevice->GetRenderTarget(0, &(renderWindow->pRenderSurface));

			if (renderWindow->getWindowParams().bZbuffer)
			    hr = pD3DDevice->GetDepthStencilSurface(&(renderWindow->pDepthStencilSurface));
			
			if (FAILED(hr)){
				assert(false);
			}

			return ;
		}

		hr = pD3DInterface->CheckDeviceFormat(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			renderWindow->getPresentationParameters()->BackBufferFormat,
			D3DUSAGE_QUERY_FILTER, D3DRTYPE_TEXTURE, 
			(D3DFORMAT)D3DTEXF_ANISOTROPIC);

		if (hr == D3DERR_NOTAVAILABLE){
			bAnisotropicFilter = false;
		}else if (hr == D3D_OK){
			bAnisotropicFilter = true;
		}

		hr = pD3DInterface->CreateDevice(
			D3DADAPTER_DEFAULT, 
			D3DDEVTYPE_HAL, 
			renderWindow->getWindowHandle(),
			D3DCREATE_HARDWARE_VERTEXPROCESSING,//硬件顶点处理
			renderWindow->getPresentationParameters(),
			&pD3DDevice);

		if (FAILED(hr)){
			hr = pD3DInterface->CreateDevice(
				D3DADAPTER_DEFAULT, 
				D3DDEVTYPE_HAL, 
				renderWindow->getWindowHandle(),
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,//软件顶点处理
				renderWindow->getPresentationParameters(),
				&pD3DDevice);
		}

		if (FAILED(hr)){
			assert(false);
		}

		pVertexBufferManager = new D3DVertexBufferManager(VERTEX_CAPACITY, VERTEX_FVF);

		pIndexBufferManager  = new D3DIndexBufferManager(VERTEX_CAPACITY);
		
		initalizeParam();
		//得到渲染面
		pD3DDevice->GetRenderTarget(0, &(renderWindow->pRenderSurface));

		if (renderWindow->getWindowParams().bZbuffer)
			pD3DDevice->GetDepthStencilSurface(&(renderWindow->pDepthStencilSurface));

		pPrimaryWindow = renderWindow;

	}else{
		assert(pD3DDevice);

		hr = pD3DDevice->CreateAdditionalSwapChain(
			renderWindow->getPresentationParameters(),
			&(renderWindow->pSwapChain)
			);
		if (FAILED(hr)){
			hr = pD3DDevice->CreateAdditionalSwapChain(
				renderWindow->getPresentationParameters(),
				&(renderWindow->pSwapChain)
				);
		}

		if (FAILED(hr)){
			assert(false);
		}

		renderWindow->pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &(renderWindow->pRenderSurface));
	
		if (renderWindow->getWindowParams().bZbuffer){
			hr = pD3DDevice->CreateDepthStencilSurface(
				renderWindow->windowParams.width,
				renderWindow->windowParams.height,
				renderWindow->D3Dpp.AutoDepthStencilFormat,
				renderWindow->D3Dpp.MultiSampleType,
				renderWindow->D3Dpp.MultiSampleQuality,
				(renderWindow->D3Dpp.Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL),
				&(renderWindow->pDepthStencilSurface),
				NULL);			
		}

		if (FAILED(hr)){
			assert(false);
		}
		
	}
}

void D3DRenderSystem::resetDevice()
{
	HRESULT hr;
	curRenderTexture = NULL;
	DeviceManager::getSingletonPtr()->beforeResetDevice();

	hr = pD3DDevice->Reset(pPrimaryWindow->getPresentationParameters());

	if (FAILED(hr)){
		assert(false);
	}
	
	clear(pPrimaryWindow->getWindowParams().bZbuffer);

	DeviceManager::getSingletonPtr()->afterResetDevice();
}

void D3DRenderSystem::beginFrame()
{
	pD3DDevice->BeginScene();
	
	pVertexBufferManager->lockBuffer();
}

void D3DRenderSystem::render()
{
	if (pVertexBufferManager->getVectexCount() > 0){
		pVertexBufferManager->unlockBuffer();

        //only quad use Index
		if (PrimType == D3DPT_TRIANGLELIST && bUseIndex){
			pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 
				pVertexBufferManager->getVectexCount(), 0, 
				pVertexBufferManager->getPrimCount());
		}else{
			pD3DDevice->DrawPrimitive(PrimType, 0, pVertexBufferManager->getPrimCount());
		}

		pVertexBufferManager->vertexCount = 0;
		pVertexBufferManager->primCount = 0;

		pVertexBufferManager->lockBuffer();
	}
}

void D3DRenderSystem::render(RenderUnit* operation)
{
	if (!operation)
		return ;

	bool bShaderParamsChange = false; //混合参数是否改变 改变了 则先调用渲染 然后设置参数
	bool bTextureStageChange = false; 
	bool bRenderTextureChange= false; //当前纹理是否改变
	Matrix4 tmpTexMat;

	Material* mtrl = operation->getMaterial();
	if (mtrl){
		Shader* shader = mtrl->getShader();
		ShaderParams tmpShaderParams;
		if (shader){
			tmpShaderParams = shader->getShaderParams();
		}

		if (curShaderParams != tmpShaderParams){
			curShaderParams = tmpShaderParams;
			bShaderParamsChange = true;
		}

		TextureMtrl* textureMtrl = mtrl->getTextureMtrl();
		if (textureMtrl){
			if (curTextureStage != textureMtrl->getTextureStage()){
				curTextureStage = textureMtrl->getTextureStage();
				bTextureStageChange = true;
			}

			tmpTexMat = textureMtrl->texMat;

			D3DTexture* texture = (D3DTexture*)textureMtrl->getTexture();
			if (texture){
				if (curRenderTexture != texture->getD3DTexture()){
					bRenderTextureChange = true;
					curRenderTexture = texture->getD3DTexture();
				}
			}else{
				assert(false);
			}

		}else{
			bRenderTextureChange = true;
			curRenderTexture = NULL;
			tmpTexMat = Matrix4::IDENTITY;
		}
	}else{
		bRenderTextureChange = true;
		curRenderTexture = NULL;	
		tmpTexMat = Matrix4::IDENTITY;	
	}

	if ( bRenderTextureChange || bTextureStageChange || bShaderParamsChange
		|| PrimType != D3DTypeConverter::toD3DPrimtiveType(operation->getOperationType())
		|| pVertexBufferManager->getVectexCount() >= VERTEX_CAPACITY - operation->getVertexCount()
		|| texMat != tmpTexMat || operation->getUseIndex() != bUseIndex){
		
		render();
		
		PrimType = D3DTypeConverter::toD3DPrimtiveType(operation->getOperationType());
		
        bUseIndex = operation->getUseIndex();

		if (bRenderTextureChange)
			pD3DDevice->SetTexture(0, curRenderTexture);
		if (bShaderParamsChange)
			setShaderParams(curShaderParams);
		if (bTextureStageChange)
			setTextureStage(curTextureStage);
		
		texMat = tmpTexMat;
		pD3DDevice->SetTransform(D3DTS_TEXTURE0, &D3DTypeConverter::toD3DMatrix(texMat));

	}
		
	pVertexBufferManager->writeBuffer(operation->getBuffer(), operation->getVertexCount(), operation->getPrimCount());

}

void D3DRenderSystem::endFrame()
{
	pVertexBufferManager->unlockBuffer();

	pD3DDevice->EndScene();
}

void D3DRenderSystem::clear(bool z)
{
	uint32 flags;
	float fZ = 0;

	flags = D3DCLEAR_TARGET;
	if (z){
		flags |= D3DCLEAR_ZBUFFER;	
		fZ = 1;
	}

	pD3DDevice->Clear(0, 0, flags, -1, fZ, 0);

}

void D3DRenderSystem::setRenderTarget(RenderTarget* target)
{
	RenderSystem::setRenderTarget(target);
}

void D3DRenderSystem::setShaderParams(const ShaderParams& shaderParams)
{
	//pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, shaderParams.AlphaBlendEnable);
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DTypeConverter::toD3DSceneBlendOperation((ShaderParams::SceneBlendOperation)shaderParams.SceneBlendOP));
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DTypeConverter::toD3DSceneBlendArgument((ShaderParams::SceneBlendArgument)shaderParams.SceneBlendSrcArg));
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DTypeConverter::toD3DSceneBlendArgument((ShaderParams::SceneBlendArgument)shaderParams.SceneBlendDesArg));
	//pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, shaderParams.AlphaTestEnable);
	pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pD3DDevice->SetRenderState(D3DRS_ALPHAREF, shaderParams.AlphaRef);
	
	//pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	RenderWindow* w = getCurRenderWindow();
	if (w){
		if (w->getWindowParams().bZbuffer){
			if (shaderParams.bUseZ){
	            pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			}else{
	            pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			}		
		}
	}
}

void D3DRenderSystem::setTextureStage(const TextureStage& textureStage)
{
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTypeConverter::toD3DTextureOperation(textureStage.AlphaBlendOP));
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTypeConverter::toD3DTextureArgument(textureStage.AlphaBlendArg1));
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTypeConverter::toD3DTextureArgument(textureStage.AlphaBlendArg2));

	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTypeConverter::toD3DTextureOperation(textureStage.ColorBlendOP));
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTypeConverter::toD3DTextureArgument(textureStage.ColorBlendArg1));
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTypeConverter::toD3DTextureArgument(textureStage.ColorBlendArg2));

	if (textureStage.wrapModeU == TextureStage::WM_Shadow && textureStage.wrapModeV == TextureStage::WM_Shadow){
		if (D3DPTADDRESSCAPS_BORDER & d3dCaps.TextureAddressCaps){
			static const DWORD ShadowBorderColor = 0xffffffff;
			pD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, ShadowBorderColor);
			pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
			pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
		}else{
			pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		}
	}else{
		pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTypeConverter::toD3DTextureAddress((TextureStage::WrapMode)textureStage.wrapModeU));
		pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTypeConverter::toD3DTextureAddress((TextureStage::WrapMode)textureStage.wrapModeV));
	}

	if (textureStage.fliterType == TextureStage::FT_Point){
		pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		//pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	}else if (textureStage.fliterType == TextureStage::FT_Line){
		pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, bAnisotropicFilter ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR);
		pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, bAnisotropicFilter ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR);
		//pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, bAnisotropicFilter ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR);
	}

	pD3DDevice->SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT);
	//textureMap
	//pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	//pD3DDevice->SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT);
}

LPDIRECT3DDEVICE9 D3DRenderSystem::getD3DDevice()
{
	return pD3DDevice;
}

LPDIRECT3D9 D3DRenderSystem::getD3DInterface()
{
	return pD3DInterface;
}

Texture* D3DRenderSystem::createTexture()
{
	return new D3DTexture;
}

void D3DRenderSystem::setProjection(float l, float r, float b, float t)
{
    D3DXMATRIX matTEMP;

	D3DXMatrixScaling(&MatProj, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&matTEMP, -0.5f, t - b + 0.5f, 0.0f);
	D3DXMatrixMultiply(&MatProj, &MatProj, &matTEMP);

	D3DXMatrixOrthoOffCenterRH(&matTEMP, l, r, b, t, 0.0f, 1.0f);//正交投影

	D3DXMatrixMultiply(&MatProj, &MatProj, &matTEMP);
	D3DXMatrixIdentity(&MatView);

	pD3DDevice->SetTransform(D3DTS_VIEW, &MatView);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &MatProj);
}

void D3DRenderSystem::prepareCanvasRender()
{
    int l = 0;
    int r = getCurRenderWindow()->getWindowParams().width;
    int b = 0;
    int t = getCurRenderWindow()->getWindowParams().height;

    setProjection((float)l, (float)r, (float)b, (float)t);
}
