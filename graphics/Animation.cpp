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
	}

	AnimFrame::AnimFrame()
		:frameTime(0)
	{

	}

	AnimFrame::~AnimFrame()
	{

	}

    void AnimFrame::move(float dx, float dy)
    {
        if (sprite)
            sprite->move(dx, dy);
    }

    void AnimFrame::moveTo(float x, float y)
    {
        if (sprite)
            sprite->moveTo(x, y);
    }

    void AnimFrame::beginScene()
    {
        if (sprite)
            sprite->beginScene();
    }

    void AnimFrame::renderScene()
    {
        if (sprite)
            sprite->renderScene();
    }

    void AnimFrame::endScene()
    {
        if (sprite)
            sprite->endScene();
    }

    void AnimFrame::setSprite(Sprite* s)
    {
        sprite = s;
    }

    Sprite* AnimFrame::getSprite()
    {
        return sprite;
    }



	HARE_IMPLEMENT_DYNAMIC_CLASS(Animation, Sprite, 0)
	{
		HARE_OBJ_LIST(animFrameList, AnimFrame)
	}

	Animation::Animation()
		:curAnimFrameID(0)
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
			animFrame = getFrame(curAnimFrameID);
			frameStartTime = 0;
		}
	}

    void Animation::beginScene()
    {
        if (!animFrame)
            return;

        frameMove();

        animFrame->beginScene();
    }

    void Animation::renderScene()
    {
        if (animFrame)
            animFrame->renderScene();
    }

    void Animation::endScene()
    {
        if (animFrame)
            animFrame->endScene();
    }

	void Animation::move(float dx, float dy)
	{
        AnimFrame::List::iterator it = animFrameList.begin();
        for (;it != animFrameList.end(); ++it){
            (*it)->move(dx, dy);
        }
	}

	void Animation::moveTo(float x, float y)
	{
        AnimFrame::List::iterator it = animFrameList.begin();
        for (;it != animFrameList.end(); ++it){
            (*it)->moveTo(x, y);
        }
	}

    int Animation::addFrame(AnimFrame* frame)
    {
        animFrameList.push_back(frame);
        curAnimFrameID = 0;
        animFrame = getFrame(curAnimFrameID);
        return animFrameList.size() - 1;
    }

    bool Animation::insertFrame(int frameID, AnimFrame* frame)
    {
        AnimFrame::List::iterator it = getFrameIT(frameID);
        
        if (it != animFrameList.end()){
            animFrameList.insert(it, frame);
            return true;
        }

        return false;
    }

    AnimFrame* Animation::getFrame(int frameID)
    {
        AnimFrame::List::iterator it = getFrameIT(frameID);

        if (it != animFrameList.end())
            return *it;
        else 
            return NULL;
    }

    bool Animation::removeFrame(int frameID)
    {
        AnimFrame::List::iterator it = getFrameIT(frameID);

        if (it != animFrameList.end()){
            animFrameList.erase(it);
            return true;
        }else{
            return false;
        } 
    }

    bool Animation::removeFrame(AnimFrame* frame)
    {
        AnimFrame::List::iterator it = std::find(animFrameList.begin(), animFrameList.end(), frame);
    
        if (it != animFrameList.end()){
            animFrameList.erase(it);
            return true;
        }else{
            return false;
        }   
    }

    bool Animation::swapFrame(int frameID_1, int frameID_2)
    {
        if (frameID_1 == frameID_2)
            return true;

        AnimFrame::Ptr anim = getFrame(frameID_1);
        if (!anim)
            return false;

        AnimFrame::List::iterator it = getFrameIT(frameID_2);
        if (it == animFrameList.end())
            return false;

        if (!removeFrame(frameID_1))
            return false;

        animFrameList.insert(it, anim);

        return false;
    }

    AnimFrame::List::iterator Animation::getFrameIT(int frameID)
    {
        if (frameID < 0)
            return animFrameList.end();

        if (frameID >= (int)animFrameList.size())
            return animFrameList.end();

        AnimFrame::List::iterator it = animFrameList.begin();
        for (int i = 0; i < frameID; ++i, ++it);

        return it;
    }

}


