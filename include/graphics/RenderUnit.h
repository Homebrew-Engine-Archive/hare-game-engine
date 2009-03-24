#ifndef RENDEROPERATION
#define RENDEROPERATION


#include "GraphicsPrerequisites.h"
#include "HareGraphicsType.h"
#include "Shader.h"


namespace hare_graphics
{
	class Shader;

	class GRAPHICS_API RenderUnit
	{
	public:
		enum RenderOperationType{
			ROT_POINT_LIST,
			ROT_LINE_LIST,
			ROT_LINE_STRIP,
			ROT_TRIANGLE_LIST,
			ROT_TRIANGLE_STRIP,
			ROT_TRIANGLE_FAN,
		};
	public:
		RenderUnit();
		virtual ~RenderUnit();

		virtual Vertex* getBuffer() = 0;
		virtual u32    getVertexCount() = 0;
		virtual u32	getPrimCount() = 0;
		virtual RenderOperationType	getOperationType() = 0;
		virtual void setTextureUVMap(f32 ul, f32 vt, f32 ur, f32 vb) = 0;
		void setShader(Shader* s)
		{
			shader = s;
		}
		Shader* getShader()
		{
			return shader;
		}
	protected:
		Shader::Ptr shader;
	};
}

#endif