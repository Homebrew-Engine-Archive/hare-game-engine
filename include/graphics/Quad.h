//***************************************************************
//  File:    Quad.h
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
#ifndef _QUAD_H_
#define _QUAD_H_


#include "GraphicsPrerequisites.h"
#include "RenderUnit.h"

namespace hare
{
	class GRAPHICS_API Quad : public RenderUnit
	{
	public:
		Quad();
		virtual ~Quad();

		virtual Vertex* getBuffer();
		virtual uint32  getVertexCount();
		virtual uint32	getPrimCount();
		virtual RenderUnit::RenderOperationType	getOperationType();
		virtual void setTextureUVMap(float ul, float vt, float ur, float vb);
		virtual void setTextureUVMap(const RectF& rc);

		void move(float dx, float dy);
		void moveTo(float x, float y);

		void setWidth(float w);
		void setHeight(float h);

		void normalize();
		void setDepth(float z);
		void setColor(uint32 color);

	public:
		Vertex buffer[4];
	};
}

#endif
