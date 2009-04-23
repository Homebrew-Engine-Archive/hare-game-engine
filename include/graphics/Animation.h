#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "GraphicsPrerequisites.h"
#include "Material.h"

namespace hare
{

	//¶¯»­frame
	class GRAPHICS_API AnimFrame : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(AnimFrame)
	public:
		AnimFrame();
		virtual ~AnimFrame();

		void frameMove();
		void setMaterial(Material* m);
		Material* getMaterial();

	public:
		float frameTime;
		RectF rectUV;

	protected:
		Material::Ptr mtrl;
	};


	class GRAPHICS_API Animation : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(Animation)
	public:
		Animation();
		virtual ~Animation();

		void addFrame(AnimFrame* frame);
		void render();
		void move(float dx, float dy);
		void moveTo(float x, float y);
		void setWidth(uint32 w);
		void setHeight(uint32 h);

	protected:
		void frameMove();

	protected:
		AnimFrame::Array animFrameList;
		uint32 curAnimFrameID;
        AnimFrame::Ptr animFrame;

		RectF rectPos;
	};

}
#endif