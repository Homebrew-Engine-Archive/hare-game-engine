//***************************************************************
//  File:    Sprite.h
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
#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "GraphicsPrerequisites.h"

namespace hare
{
    class GRAPHICS_API Sprite : public Object
	{
        HARE_DECLARE_ABSTRACT_CLASS(Sprite)
	public:
		Sprite();
		virtual ~Sprite();

        virtual void move(float dx, float dy);

        virtual void moveTo(float x, float y);

		virtual void render() = 0;

        virtual const PointF& getPosition();
    
        void setOrigoPos(float x, float y);

        void moveOrigoPos(float dx, float dy);

        const PointF& getOrigoPos();

        void setFaceX(bool b);

        void setFaceY(bool b);

        void setSpriteName(const String& name);

        const String& getSpriteName();

        void setColor(uint32 c);

        uint32 getColor();

        void setZ(float z);

        float getZ();

    protected:
        PointF origoPos;
        PointF pos;
        bool   bFaceX;
        bool   bFaceY;
        String spriteName;
        uint32 color;
        float  zDepth;
    
    };
}

#endif
