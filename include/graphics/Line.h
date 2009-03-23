#ifndef LINE
#define LINE

#include "GraphicsPrerequisites.h"
#include "RenderOperation.h"


namespace hare_graphics
{
	class GRAPHICS_API Line : public RenderOperation
	{
	public:
		Line();
		virtual ~Line();

		virtual Vertex* getBuffer();
		virtual u32 getVertexCount();
		virtual u32	getPrimCount();
		virtual RenderOperation::RenderOperationType	getOperationType();
		virtual void setTextureUVMap(f32 ul, f32 vt, f32 ur, f32 vb);

		void setColor(u32 color);
		void set(int x1, int y1, int x2, int y2, u32 color = -1, f32 z = 0.f);
		void normalize();
	public:
		Vertex buffer[2];
	};
}

#endif