//***************************************************************
//  File:    Animation.h
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
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "GraphicsPrerequisites.h"
#include "Sprite.h"

namespace hare
{

	//����frame
	class GRAPHICS_API AnimFrame : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(AnimFrame)
	public:
		AnimFrame();
		virtual ~AnimFrame();

        void move(float dx, float dy);

        void moveTo(float x, float y);

        void beginScene();

        void renderScene();

        void endScene();

		void setSprite(Sprite* s);

		Sprite* getSprite();

	public:
		float frameTime;

	protected:
		Sprite::Ptr sprite;
	};


	class GRAPHICS_API AnimationSprite : public Sprite
	{
		HARE_DECLARE_DYNAMIC_CLASS(AnimationSprite)
	public:
		AnimationSprite();
		virtual ~AnimationSprite();

		int addFrame(AnimFrame* frame);

        AnimFrame* getFrame(int frameID);

        bool insertFrame(int frameID, AnimFrame* frame);

        bool removeFrame(int frameID);

        bool removeFrame(AnimFrame* frame);

        bool swapFrame(int frameID_1, int frameID_2);

        virtual void move(float dx, float dy);

        virtual void moveTo(float x, float y);

        virtual void beginScene();

        virtual void renderScene();

        virtual void endScene();

	protected:
		void frameMove();
        AnimFrame::List::iterator getFrameIT(int frameID);

	protected:
		AnimFrame::List animFrameList;
		uint32 curAnimFrameID;
        AnimFrame::Ptr animFrame;

	};

}
#endif