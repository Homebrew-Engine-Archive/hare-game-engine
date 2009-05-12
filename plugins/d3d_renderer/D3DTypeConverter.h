//***************************************************************
//  File:    D3DTypeConverter.h
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
#ifndef _D3DTYPECONVERTER_H_
#define _D3DTYPECONVERTER_H_


class D3DTypeConverter
{
public:
	static D3DBLENDOP   toD3DSceneBlendOperation(ShaderParams::SceneBlendOperation sceneOP);
	static D3DBLEND     toD3DSceneBlendArgument(ShaderParams::SceneBlendArgument sceneArg);
	static D3DTEXTUREOP toD3DTextureOperation(TextureStage::ColorAlphaBlendOperation texOP);
	static uint32       toD3DTextureArgument(TextureStage::ColorAlphaBlendArgument texArg);
	static D3DTEXTUREADDRESS toD3DTextureAddress(TextureStage::WrapMode wrapMod);
	static D3DFORMAT    toD3DFormat(HarePixelFormat hpf);
	static D3DPRIMITIVETYPE toD3DPrimtiveType(RenderUnit::RenderOperationType primtype);
	static D3DMATRIX    toD3DMatrix(const Matrix4& mat);
};


#endif 