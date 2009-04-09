#ifndef QUAD
#define QUAD


#include "GraphicsPrerequisites.h"
#include "RenderUnit.h"

namespace hare_graphics
{
	class GRAPHICS_API Quad : public RenderUnit
	{
	public:
		Quad();
		virtual ~Quad();

		virtual Vertex* getBuffer();
		virtual u32 getVertexCount();
		virtual u32	getPrimCount();
		virtual RenderUnit::RenderOperationType	getOperationType();
		virtual void setTextureUVMap(f32 ul, f32 vt, f32 ur, f32 vb);
		virtual void setTextureUVMap(const RectF& rc);

		void move(float dx, float dy);
		void moveTo(float x, float y);

		void setWidth(float w);
		void setHeight(float h);

		void normalize();
		void setDepth(f32 z);
		void setColor(u32 color);

	public:
		Vertex buffer[4];
	};
}

#endif
