//***************************************************************
//  File:    GLTypeConverter.h
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
#ifndef _GLTYPECONVERTER_H_
#define _GLTYPECONVERTER_H_


class GLTypeConverter
{
public:
    static GLenum toGLPrimtiveType(RenderUnit::RenderOperationType primtype);
    static uint32 countByPrimtType(GLenum primtype);
    static GLenum toGLFormat(HarePixelFormat hpf);
    static GLenum getGLInternalFormat(HarePixelFormat format);
	static GLenum toGLTextureWrapMode(TextureStage::WrapMode wrapMode);
	static GLenum toGLSceneBlendArg(ShaderParams::SceneBlendArgument sceneBlendArg);
};


#endif
