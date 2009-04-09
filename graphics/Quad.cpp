#include "PCH.h"
#include "Quad.h"

namespace hare_graphics
{
	Quad::Quad()
	{
		normalize();
 	}

	Quad::~Quad()
	{

	}

	Vertex* Quad::getBuffer()
	{
		return buffer;
	}

	u32 Quad::getVertexCount()
	{
		return 4;
	}

	u32 Quad::getPrimCount()
	{
		return 2;
	}

	RenderUnit::RenderOperationType Quad::getOperationType()
	{
		return RenderUnit::ROT_TRIANGLE_LIST;
	}

	void Quad::move(float dx, float dy)
	{
		for (int i = 0; i < 4; ++i){
			buffer[i].x += dx;
			buffer[i].y += dy;
		}
	}

	void Quad::moveTo(float x, float y)
	{
		float width = buffer[3].x - buffer[0].x;
		float height= buffer[1].y - buffer[0].y;

		buffer[0].x = x;
		buffer[0].y = y;
		buffer[1].x = x;
		buffer[1].y = y + height;
		buffer[2].x = x + width;
		buffer[2].y = y + height;
		buffer[3].x = x + width;
		buffer[3].y = y;
	}

	void Quad::setWidth(float w)
	{
		buffer[2].x = buffer[3].x = buffer[0].x + w;
	}

	void Quad::setHeight(float h)
	{
		buffer[1].y = buffer[2].y = buffer[0].y + h;
	}

	void Quad::normalize()
	{
		buffer[0].u = buffer[0].x = 0;		// 0.....3     创建索引时  0 1 2  2 3 0
		buffer[0].v = buffer[0].y = 0;		// . .   .
		buffer[1].u = buffer[1].x = 0;		// .  .  .
		buffer[1].v = buffer[1].y = 1;		// .   . .		
		buffer[2].u = buffer[2].x = 1;		// 1.....2
		buffer[2].v = buffer[2].y = 1;
		buffer[3].u = buffer[3].x = 1;
		buffer[3].v = buffer[3].y = 0;
		
		buffer[0].z = buffer[1].z = buffer[2].z = buffer[3].z = 0;
		buffer[0].diffuse = buffer[1].diffuse = 
		buffer[2].diffuse = buffer[3].diffuse = -1;
	}
	void Quad::setTextureUVMap(f32 ul, f32 vt, f32 ur, f32 vb)
	{
		buffer[0].u = ul;	
		buffer[0].v = vt;		
		buffer[1].u = ul;		
		buffer[1].v = vb;		
		buffer[2].u = ur;		
		buffer[2].v = vb;
		buffer[3].u = ur;
		buffer[3].v = vt;
	}

	void Quad::setTextureUVMap(const RectF& rc)
	{
		buffer[0].u = rc.minX;	
		buffer[0].v = rc.minY;		
		buffer[1].u = rc.minX;		
		buffer[1].v = rc.maxY;		
		buffer[2].u = rc.maxX;		
		buffer[2].v = rc.maxY;
		buffer[3].u = rc.maxX;
		buffer[3].v = rc.minY;	
	}

	void Quad::setDepth(f32 z)
	{
		buffer[0].z = buffer[1].z = buffer[2].z = buffer[3].z = z;
	}

	void Quad::setColor(u32 color)
	{
		buffer[0].diffuse = buffer[1].diffuse = 
		buffer[2].diffuse = buffer[3].diffuse = color;
	}
}
