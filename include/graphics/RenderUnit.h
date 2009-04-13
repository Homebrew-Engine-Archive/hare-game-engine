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
			ROT_LINE_LIST,
			ROT_TRIANGLE_LIST,
            ROT_QUAD_LIST,
		};
	public:
		RenderUnit();
		virtual ~RenderUnit();

		virtual Vertex* getBuffer() = 0;
		virtual uint32  getVertexCount() = 0;
		virtual uint32  getPrimCount() = 0;
		virtual RenderOperationType	getOperationType() = 0;
		void setMaterial(Material* m)
		{
			mtrl = m;
		}

		Material* getMaterial()
		{
			return mtrl;
		}

        bool getUseIndex()
        {
            return bUseIndex;
        }

	protected:
		Material::Ptr mtrl;
        bool bUseIndex;
	};
}

#endif
