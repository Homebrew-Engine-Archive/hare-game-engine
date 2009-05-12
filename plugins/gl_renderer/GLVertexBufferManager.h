//***************************************************************
//  File:    GLVertexBufferManager.h
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
#ifndef _GLVERTEXBUFFERMANAGER_H_
#define _GLVERTEXBUFFERMANAGER_H_

#define VERTEX_BUFFER_COUNT 1024


class GLVertexBufferManager : public Singleton<GLVertexBufferManager>
{
	HARE_DECLARE_SINGLETON(GLVertexBufferManager)

    friend class GLRenderSystem;
public:
    GLVertexBufferManager();
	~GLVertexBufferManager();

	void writeBuffer(Vertex* buffer, uint32 count);

	float* getVertexArray();
	float* getTexCoordArray();
	uint8* getColorArray();

    uint32 getArrayCount();

    uint32 getVertexVBO();
	uint32 getTexCoordVBO();
	uint32 getColorVBO();

	void lock();
	void unlock();
	
protected:
	float vertexArray[VERTEX_BUFFER_COUNT * 3];
	float texCoordArray[VERTEX_BUFFER_COUNT * 2];
	uint8 colorArray[VERTEX_BUFFER_COUNT * 4];

	uint32 arrayCount;

	uint32 VBOVertex;
	uint32 VBOTexCoord;
	uint32 VBOColor;
};
#endif
