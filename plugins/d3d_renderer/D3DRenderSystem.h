//***************************************************************
//  File:    D3DRenderSystem.h
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
#ifndef _D3DRENDERSYSTEM_H_
#define _D3DRENDERSYSTEM_H_

class D3DRenderWindow;
class D3DVertexBufferManager;
class D3DIndexBufferManager;
class DeviceManager;


class D3DRenderSystem : public RenderSystem
{
public:
	D3DRenderSystem();
	virtual ~D3DRenderSystem();

	virtual void initalize();//配置信息 以后加
	void createDevice(D3DRenderWindow* renderWindow);
	void resetDevice();

	virtual void release();

	virtual void beginFrame();
	virtual void render();
	virtual void render(RenderUnit* operation);
	virtual void endFrame();
	virtual void clear(bool z);

	virtual void setRenderTarget(RenderTarget* target);
	virtual void setShaderParams(const ShaderParams& shaderParams);
	virtual void setTextureStage(const TextureStage& textureStage);

	virtual Texture* createTexture();
    virtual void setProjection(float l, float r, float b, float t);
    virtual void prepareCanvasRender();

	LPDIRECT3DDEVICE9 getD3DDevice();

	LPDIRECT3D9 getD3DInterface();

	static const uint32 VERTEX_CAPACITY;

	static const uint32 VERTEX_FVF;
protected:
	void initalizeParam();

private:

	LPDIRECT3D9 pD3DInterface;

	LPDIRECT3DDEVICE9 pD3DDevice;

	D3DCAPS9 d3dCaps;

	D3DVertexBufferManager* pVertexBufferManager; 

	D3DIndexBufferManager* pIndexBufferManager;

	ShaderParams curShaderParams;

	TextureStage curTextureStage;

	LPDIRECT3DTEXTURE9 curRenderTexture;

	bool bAnisotropicFilter;

	D3DPRIMITIVETYPE PrimType; //当前渲染类型 例如 点 线 三角形 等等

	D3DRenderWindow* pPrimaryWindow;

	DeviceManager* pDeviveManager;

	Matrix4 texMat;

    bool bUseIndex;

    D3DXMATRIX MatView;
	D3DXMATRIX MatProj;
};


#endif