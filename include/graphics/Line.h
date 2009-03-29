#ifndef LINE
#define LINE

#include "GraphicsPrerequisites.h"
#include "RenderUnit.h"


namespace hare_graphics
{
	class GRAPHICS_API Line : public RenderUnit
	{
	public:
		Line();
		virtual ~Line();

		virtual Vertex* getBuffer();
		virtual u32 getVertexCount();
		virtual u32	getPrimCount();
		virtual RenderUnit::RenderOperationType	getOperationType();

		void setColor(u32 color);
		void set(int x1, int y1, int x2, int y2, u32 color = -1, f32 z = 0.f);
		void normalize();
	public:
		Vertex buffer[2];
	};
}

#endif