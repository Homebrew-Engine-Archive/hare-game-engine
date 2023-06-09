//***************************************************************
//  File:    SceneManager.h
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
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "GraphicsPrerequisites.h"
#include "SceneListener.h"
#include "Sprite.h"

namespace hare
{
	class SceneListener;

    class GRAPHICS_API SceneManager : public Object
	{
        HARE_DECLARE_DYNAMIC_CLASS(SceneManager)
	public:
		SceneManager();
	    virtual ~SceneManager();

		void beginScene();

		void renderScene();

		void endScene();

		int  addSprite(Sprite* sprite);

		void removeSprite(Sprite* sprite);

        void removeAllSprite();

		void setSceneListener(SceneListener* listener);

        Sprite* getSpriteByName(const String& name);

        Sprite* getSpriteByID(int id);

        int getSpriteCount();

	protected:
        Sprite::List spriteList;
        SceneListener::Ptr sceneListener;

	};
}

#endif
