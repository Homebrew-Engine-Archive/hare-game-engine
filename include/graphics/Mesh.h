//***************************************************************
//  File:    Mesh.h
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
#ifndef _MESH_H_
#define _MESH_H_

#include "GraphicsPrerequisites.h"
#include "RenderUnit.h"


namespace hare
{
	class GRAPHICS_API Mesh : public RenderUnit
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual Vertex* getBuffer();
		virtual uint32     getVertexCount();
		virtual uint32	    getPrimCount();
		virtual RenderUnit::RenderOperationType	getOperationType();
		virtual void setTextureUVMap(float ul, float vt, float ur, float vb);

		void createMesh(uint32 r, uint32 c, uint32 unitWidth = 1, int left = 0, int top = 0, float z = 0);
		void createUserMesh(uint32 count);

	protected:
		DataHolder buffer;

	};
}

#endif
