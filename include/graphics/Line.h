#ifndef _LINE_H_
#define _LINE_H_

#include "GraphicsPrerequisites.h"
#include "RenderUnit.h"


namespace hare
{
	class GRAPHICS_API Line : public RenderUnit
	{
	public:
		Line();
		virtual ~Line();

		virtual Vertex* getBuffer();
		virtual uint32 getVertexCount();
		virtual uint32	getPrimCount();
		virtual RenderUnit::RenderOperationType	getOperationType();

		void setColor(uint32 color);
		void set(float x1, float y1, float x2, float y2, uint32 color = -1, float z = 0.f);
		void normalize();
	public:
		Vertex buffer[2];
	};
}

#endif
