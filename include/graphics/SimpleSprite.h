//***************************************************************
//  File:    SimpleSprite.h
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
#ifndef _SIMPLESPRITE_H_
#define _SIMPLESPRITE_H_

#include "Sprite.h"
#include "Quad.h"
#include "Shader.h"
#include "Particle.h"

#include "GraphicsPrerequisites.h"

namespace hare
{
	class TextureMtrl;
	class Texture;
	class AnimModUnit;

	class GRAPHICS_API SimpleSprite : public Sprite
	{
	public:
		SimpleSprite();
		virtual ~SimpleSprite();

		virtual void render();

		virtual void move(float dx, float dy);

        virtual void moveTo(float x, float y);

		void loadFromImage(const String& filename);

	protected:
		Quad			 quad;
		Shader*		     shader;
		Particle::Ptr    particle;

		//renderToTex test
		Texture*     _tex;
		TextureMtrl::Ptr _texMtrl;
	};
}

#endif
