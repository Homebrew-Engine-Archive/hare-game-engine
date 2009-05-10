//***************************************************************
//  File:    Line.cpp
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
#include "Line.h"

namespace hare
{
	Line::Line()
	{
		normalize();
 	}

	Line::~Line()
	{

	}

	Vertex* Line::getBuffer()
	{
		return buffer;
	}

	uint32 Line::getVertexCount()
	{
		return 2;
	}

	uint32 Line::getPrimCount()
	{
		return 1;
	}

	RenderUnit::RenderOperationType Line::getOperationType()
	{
		return RenderUnit::ROT_LINE_LIST;
	}

	void Line::normalize()
	{
		buffer[0].u = buffer[0].x = 0;
		buffer[0].v = buffer[0].y = 0;
		buffer[1].u = buffer[1].x = 1;
		buffer[1].v = buffer[1].y = 1;


		buffer[0].z = buffer[1].z = 0;
		buffer[0].diffuse = buffer[1].diffuse = -1;
	}

	void Line::setColor(uint32 color)
	{
		buffer[0].diffuse = buffer[1].diffuse = color;
	}

	void Line::set(float x1, float y1, float x2, float y2, uint32 color, float z)
	{
		buffer[0].x = x1;
		buffer[0].y = y1;
		buffer[1].x = x2;
		buffer[1].y = y2;
		buffer[0].z = buffer[1].z = z;
		setColor(color);
	}
}
