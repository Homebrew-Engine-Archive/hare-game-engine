//***************************************************************
//  File:    GLVertexBufferManager.cpp
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
#include "GLVertexBufferManager.h"
#include "GLUtility.h"




HARE_IMPLEMENT_SINGLETON(GLVertexBufferManager)

GLVertexBufferManager::GLVertexBufferManager()
    :arrayCount(0)
    ,VBOVertex(0)
    ,VBOTexCoord(0)
    ,VBOColor(0)
{
	glGenBuffersARB(1, &VBOVertex);
	glGenBuffersARB(1, &VBOTexCoord);
	glGenBuffersARB(1, &VBOColor);
}

GLVertexBufferManager::~GLVertexBufferManager()
{
    glDeleteBuffersARB(1, &VBOVertex);
    glDeleteBuffersARB(1, &VBOTexCoord);
    glDeleteBuffersARB(1, &VBOColor);
}

void GLVertexBufferManager::writeBuffer(Vertex* buffer, uint32 count)
{
	Color color;
	for (uint32 i = 0; i < count; ++i){
        vertexArray[arrayCount * 3 + 0] = buffer[i].x;
		vertexArray[arrayCount * 3 + 1] = buffer[i].y;
		vertexArray[arrayCount * 3 + 2] = buffer[i].z;
        texCoordArray[arrayCount * 2 + 0] = buffer[i].u;
        texCoordArray[arrayCount * 2 + 1] = buffer[i].v;
        color = buffer[i].diffuse;
        colorArray[arrayCount * 4 + 0] = (uint8)(255 * color.R);
        colorArray[arrayCount * 4 + 1] = (uint8)(255 * color.G);
        colorArray[arrayCount * 4 + 2] = (uint8)(255 * color.B);
        colorArray[arrayCount * 4 + 3] = (uint8)(255 * color.A);
		arrayCount++;
	}
}

void GLVertexBufferManager::lock()
{
	// NB: GL_STATIC_DRAW_ARB and GL_STREAM_DRAW_ARB may use video memory
	//     GL_DYNAMIC_DRAW_ARB may use AGP memory
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOVertex);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, arrayCount * 3 * sizeof(float), vertexArray, GL_STATIC_DRAW_ARB);
	glVertexPointer(3, GL_FLOAT, 0, NULL);		
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOTexCoord );
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, arrayCount * 2 * sizeof(float), texCoordArray, GL_STATIC_DRAW_ARB);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOColor);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, arrayCount * 4 * sizeof(uint8), colorArray, GL_STATIC_DRAW_ARB);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, NULL);
}

void GLVertexBufferManager::unlock()
{
	glDisableClientState(GL_VERTEX_ARRAY);				
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

    arrayCount = 0;
}

float* GLVertexBufferManager::getVertexArray()
{
	return vertexArray;
}

float* GLVertexBufferManager::getTexCoordArray()
{
	return texCoordArray;
}

uint8* GLVertexBufferManager::getColorArray()
{
	return colorArray;
}

uint32 GLVertexBufferManager::getVertexVBO()
{
	return VBOVertex;
}

uint32 GLVertexBufferManager::getTexCoordVBO()
{
	return VBOTexCoord;
}

uint32 GLVertexBufferManager::getColorVBO()
{
	return VBOColor;
}

uint32 GLVertexBufferManager::getArrayCount()
{
	return arrayCount;
}
