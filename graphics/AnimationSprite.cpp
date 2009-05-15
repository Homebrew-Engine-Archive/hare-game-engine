//***************************************************************
//  File:    AnimationSprite.cpp
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "AnimationSprite.h"
#include "RenderSystem.h"
#include "Quad.h"
#include "Canvas.h"

namespace hare
{
	HARE_IMPLEMENT_DYNAMIC_CLASS(AnimFrame, Object, 0)
	{
        HARE_META(frameTime, float)
        HARE_OBJ(sprite, Sprite)
	}

	AnimFrame::AnimFrame()
		:frameTime(1)
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



	HARE_IMPLEMENT_DYNAMIC_CLASS(AnimationSprite, Sprite, 0)
	{
		HARE_OBJ_ARRAY(animFrameList, AnimFrame)
	}

	AnimationSprite::AnimationSprite()
		:curAnimFrameID(0)
        ,bStop(true)
        ,bPause(false)
        ,bPlayOnce(false)
        ,frameStartTime(0)
	{

	}

	AnimationSprite::~AnimationSprite()
	{

	}

	void AnimationSprite::frameMove()
	{
		if (!animFrame)
			return;

        if (bStop)
            return;

        if (bPause)
            return;

		frameStartTime += getTimer().getDeltaTime();

		if (frameStartTime >= animFrame->frameTime){
			curAnimFrameID = (curAnimFrameID + 1) % animFrameList.size();
			animFrame = getFrame(curAnimFrameID);
			frameStartTime = 0;
            if (bPlayOnce && curAnimFrameID == 0){
                stop();
            }
		}
	}

    void AnimationSprite::beginScene()
    {
        if (!animFrame)
            return;

        frameMove();

        animFrame->beginScene();
    }

    void AnimationSprite::renderScene()
    {
        moveTo(pos.x, pos.y);
        if (animFrame)
            animFrame->renderScene();
    }

    void AnimationSprite::endScene()
    {
        if (animFrame)
            animFrame->endScene();
    }

	void AnimationSprite::move(float dx, float dy)
	{
        AnimFrame::Array::iterator it = animFrameList.begin();
        for (;it != animFrameList.end(); ++it){
            (*it)->move(dx, dy);
        }
	}

	void AnimationSprite::moveTo(float x, float y)
	{
        AnimFrame::Array::iterator it = animFrameList.begin();
        for (;it != animFrameList.end(); ++it){
            float origoX = origoPos.x;
            float origoY = origoPos.y;

            if (!bFaceX){
                origoX = -origoX;
            }

            if (!bFaceY){
                origoY = -origoY;
            }

            (*it)->moveTo(x + origoX, y + origoY);
        }
	}

    int AnimationSprite::addFrame(AnimFrame* frame)
    {
        animFrameList.push_back(frame);
        resetAnimation();
        return animFrameList.size() - 1;
    }

    bool AnimationSprite::insertFrame(int frameID, AnimFrame* frame)
    {
        AnimFrame::Array::iterator it = getFrameIT(frameID);
        
        if (it != animFrameList.end()){
            animFrameList.insert(it, frame);
            resetAnimation();
            return true;
        }

        return false;
    }

    AnimFrame* AnimationSprite::getFrame(int frameID)
    {
        if (frameID < 0)
            return NULL;

        if (frameID >= (int)animFrameList.size())
            return NULL;

        return animFrameList[frameID];
    }

    bool AnimationSprite::removeFrame(int frameID)
    {
        AnimFrame::Array::iterator it = getFrameIT(frameID);

        if (it != animFrameList.end()){
            animFrameList.erase(it);
            resetAnimation();
            return true;
        }else{
            return false;
        } 
    }

    bool AnimationSprite::removeFrame(AnimFrame* frame)
    {
        AnimFrame::Array::iterator it = std::find(animFrameList.begin(), animFrameList.end(), frame);
    
        if (it != animFrameList.end()){
            animFrameList.erase(it);
            resetAnimation();
            return true;
        }else{
            return false;
        }   
    }

    bool AnimationSprite::swapFrame(int frameID_1, int frameID_2)
    {
        if (frameID_1 == frameID_2)
            return true;

        AnimFrame::Ptr anim_1 = getFrame(frameID_1);
        if (!anim_1)
            return false;

        AnimFrame::Ptr anim_2 = getFrame(frameID_2);
        if (!anim_2)
            return false;

        animFrameList[frameID_1] = anim_2;
        animFrameList[frameID_2] = anim_1;

        return false;
    }

    uint32 AnimationSprite::getFrameCount()
    {
        return animFrameList.size();
    }

    AnimFrame::Array::iterator AnimationSprite::getFrameIT(int frameID)
    {
        if (frameID < 0)
            return animFrameList.end();

        if (frameID >= (int)animFrameList.size())
            return animFrameList.end();

        AnimFrame::Array::iterator it = animFrameList.begin();
        for (int i = 0; i < frameID; ++i, ++it);

        return it;
    }

    void AnimationSprite::resetAnimation()
    {
        curAnimFrameID = 0;
        animFrame = getFrame(curAnimFrameID);
        frameStartTime = 0;
    }

    void AnimationSprite::play()
    {
        bStop = false;
        bPlayOnce = false;
        bPause = false;
        resetAnimation();
    }

    void AnimationSprite::playAction()
    {
        bStop = false;
        bPlayOnce = true;
        bPause = false;
        resetAnimation();
    }

    void AnimationSprite::pause()
    {
        bPause = true;
    }

    void AnimationSprite::resume()
    {
        bPause = false;
    }

    void AnimationSprite::stop()
    {
        bPause = false;
        bStop = true;
        resetAnimation();
    }

}


