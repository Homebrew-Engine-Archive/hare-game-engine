#include "PCH.h"
#include "Line.h"

namespace hare_graphics
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

	u32 Line::getVertexCount()
	{
		return 2;
	}

	u32 Line::getPrimCount()
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

	void Line::setColor(u32 color)
	{
		buffer[0].diffuse = buffer[1].diffuse = color;
	}

	void Line::set(int x1, int y1, int x2, int y2, u32 color, f32 z)
	{
		buffer[0].x = (f32)x1;		
		buffer[0].y = (f32)y1;		
		buffer[1].x = (f32)x2;		
		buffer[1].y = (f32)y2;
		buffer[0].z = buffer[1].z = z;
		setColor(color);
	}
}