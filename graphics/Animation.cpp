#include "PCH.h"
#include "Animation.h"
#include "RenderSystem.h"
#include "Quad.h"
#include "Canvas.h"

namespace hare
{
	HARE_IMPLEMENT_DYNAMIC_CLASS(AnimFrame, Object, 0)
	{
        HARE_META(frameTime, float)
        HARE_META_F(rectUV, RectF, propHide)
	}

	AnimFrame::AnimFrame()
		:frameTime(0)
		,rectUV(0,0,1,1)
	{

	}

	AnimFrame::~AnimFrame()
	{

	}

	void AnimFrame::frameMove()
	{
		if (!mtrl)
			return ;

		TextureMtrl* texMtrl = mtrl->getTextureMtrl();
		if (texMtrl){
			Matrix4 mat;
			mat = Matrix4::IDENTITY;

			mat._11 = rectUV.width();
			mat._22 = rectUV.height();
			mat._13 = rectUV.minX;
			mat._23 = rectUV.minY;

			texMtrl->texMat = mat;
		}
	}

	void AnimFrame::setMaterial(Material* m)
	{
        mtrl = m;
	}

	Material* AnimFrame::getMaterial()
	{
        return mtrl;
	}



	HARE_IMPLEMENT_DYNAMIC_CLASS(Animation, Object, 0)
	{
		HARE_OBJ_ARRAY(animFrameList, AnimFrame)
	}

	Animation::Animation()
		:curAnimFrameID(0)
		,rectPos(0, 0, 0, 0)
	{

	}

	Animation::~Animation()
	{

	}

	void Animation::frameMove()
	{
		static float frameStartTime = 0;

		if (!animFrame)
			return;

		frameStartTime += getTimer().getDeltaTime();

		if (frameStartTime >= animFrame->frameTime){
			curAnimFrameID = (curAnimFrameID + 1) % animFrameList.size();
			animFrame = animFrameList[curAnimFrameID];
			frameStartTime = 0;
		}

		animFrame->frameMove();
	}

	void Animation::addFrame(AnimFrame* frame)
	{
        animFrameList.push_back(frame);
        curAnimFrameID = 0;
        animFrame = animFrameList[curAnimFrameID];
	}

	void Animation::render()
	{
		if (!animFrame)
			return;

        frameMove();

		if (rectPos.isEmpty()){
            getCanvas()->drawImage(rectPos.minX, rectPos.minY, animFrame->getMaterial());		
		}else{
		    getCanvas()->drawImage(rectPos, animFrame->getMaterial());
		}
	}

	void Animation::move(float dx, float dy)
	{
		rectPos.move(dx, dy);
	}

	void Animation::moveTo(float x, float y)
	{
        rectPos.moveTo(x, y);
	}
}


