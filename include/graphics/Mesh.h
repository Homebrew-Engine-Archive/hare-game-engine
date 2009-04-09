#ifndef _MESH_H_
#define _MESH_H_

#include "GraphicsPrerequisites.h"
#include "RenderUnit.h"
#include "DataHolder.h"

namespace hare_graphics
{
	class GRAPHICS_API Mesh : public RenderUnit
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual Vertex* getBuffer();
		virtual u32     getVertexCount();
		virtual u32	    getPrimCount();
		virtual RenderUnit::RenderOperationType	getOperationType();
		virtual void setTextureUVMap(f32 ul, f32 vt, f32 ur, f32 vb);

		void createMesh(u32 r, u32 c, u32 unitWidth = 1, int left = 0, int top = 0, f32 z = 0);
		void createUserMesh(u32 count);

	protected:
		DataHolder buffer;
		
	};
}

#endif
