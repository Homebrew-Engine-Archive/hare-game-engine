//***************************************************************
//  File:    RenderSystem.h
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
#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

#include "GraphicsPrerequisites.h"
#include "Material.h"
#include "Shader.h"

namespace hare
{
	class RenderTarget;
	class RenderWindow;
	class Texture;
	class RenderUnit;


	class GRAPHICS_API RenderSystem : public Singleton<RenderSystem>
	{
		HARE_DECLARE_SINGLETON(RenderSystem)
	public:
		RenderSystem();
		virtual ~RenderSystem();

		virtual void initalize() = 0;
		virtual void release() = 0;

		virtual void beginFrame() = 0;
		virtual void render() = 0;
		virtual void render(RenderUnit* operation) = 0;
		virtual void endFrame() = 0;
		virtual void clear(bool z) = 0;

		virtual void setShaderParams(const ShaderParams& shaderParams) = 0;
		virtual void setTextureStage(const TextureStage& textureStage) = 0;

		void setRenderTarget(RenderTarget* target);
		void setCurRenderWindow(RenderWindow* window);
		RenderTarget* getRenderTarget();
		RenderWindow* getCurRenderWindow();

		virtual Texture* createTexture() = 0;
        virtual void setProjection(float l, float r, float b, float t) = 0;
        virtual void prepareCanvasRender() = 0;

	protected:
		RenderTarget* curRenderTarget;
		RenderWindow* curRenderWindow;

	};
}

#endif
