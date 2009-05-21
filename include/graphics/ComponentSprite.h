//***************************************************************
//  File:    ComponentSprite.h
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
#ifndef _COMPONENTSPRITE_H_
#define _COMPONENTSPRITE_H_

#include "GraphicsPrerequisites.h"
#include "Sprite.h"

namespace hare
{
    class GRAPHICS_API ComponentSprite : public Sprite
    {
        HARE_DECLARE_DYNAMIC_CLASS(ComponentSprite)
    public:
        ComponentSprite();
		virtual ~ComponentSprite();

        virtual void move(float dx, float dy);

        virtual void moveTo(float x, float y);

		virtual void render();

        int addPart(Sprite* s);

        int getPartID(Sprite* s);

        bool removePart(int partID);

        bool removePart(Sprite* s);

        //NB: swap render sequence
        bool swapPart(int partID_1, int partID_2);

        Sprite* getPart(int partID);

        Sprite* getPart(const String& name);

        uint32 getPartCount();
    
    protected:
        Sprite::List::iterator getPartIT(int partID);

        Sprite::List components;
    };
}

#endif
