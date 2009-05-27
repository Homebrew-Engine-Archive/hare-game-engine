//***************************************************************
//  File:    GLRenderSystem.h
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
#ifndef _GLRENDERSYSTEM_H_
#define _GLRENDERSYSTEM_H_

class GLVertexBufferManager;

class GLRenderSystem : public RenderSystem
{
public:
    GLRenderSystem();
    virtual ~GLRenderSystem();

    virtual void initalize();
    virtual void release();

    virtual void beginFrame();
    virtual void render();
    virtual void render(RenderUnit* operation);
    virtual void endFrame();
    virtual void clear(bool z);

    virtual void setShaderParams(const ShaderParams& shaderParams);
    virtual void setTextureStage(const TextureStage& textureStage);

    virtual Texture* createTexture();
    virtual void setProjection(float l, float r, float b, float t);
    virtual void prepareCanvasRender();

	void initalizeParam(bool bZBuffer);

    void resetShaderTextureStage();

    GLuint getCurTexture();

protected:
    void makeGLMatrix(GLfloat gl_matrix[16], const Matrix4& mat);

protected:
	ShaderParams curShaderParams;

	TextureStage curTextureStage;

	GLuint  curRenderTexture;

	GLenum  PrimType;

	Matrix4 texMat;

	GLfloat gl_matrix[16];
};


#endif
