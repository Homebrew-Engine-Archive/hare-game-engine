//***************************************************************
//  File:    Sprite.cpp
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
#include "Sprite.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(Sprite, Object, 0)
    {
        HARE_META(origoPos, PointF)
        HARE_META(pos, PointF)
        HARE_META(spriteName, String)
        HARE_META_F(color, uint32, propColor)
        HARE_META(zDepth, float)
    }

	Sprite::Sprite()
        :origoPos(0, 0)
        ,pos(0,0)
        ,bFaceX(true)
        ,bFaceY(true)
        ,color(0xffffffff)
        ,zDepth(0)
	{

	}
	Sprite::~Sprite()
	{

	}

    void Sprite::setOrigoPos(float x, float y)
    {
        origoPos.x = x;origoPos.y = y;
    }

    void Sprite::moveOrigoPos(float dx, float dy)
    {
        origoPos.move(dx, dy);
    }

    const PointF& Sprite::getOrigoPos()
    {
        return origoPos;
    }

    void Sprite::move(float dx, float dy)
    {
        pos.x += dx; pos.y += dy;
    }

    void Sprite::moveTo(float x, float y)
    {
        pos.x = x; pos.y = y;
    }

    const PointF& Sprite::getPosition()
    {
        return pos;
    }

    void Sprite::setFaceX(bool b)
    {
        bFaceX = b;
    }

    void Sprite::setFaceY(bool b)
    {
        bFaceY = b;
    }

    void Sprite::setSpriteName(const String& name)
    {
        spriteName = name;
    }

    const String& Sprite::getSpriteName()
    {
        return spriteName;
    }

    void Sprite::setColor(uint32 c)
    {
        color = c;
    }

    uint32 Sprite::getColor()
    {
        return color;
    }

    void Sprite::setZ(float z)
    {
        zDepth = z;
    }

    float Sprite::getZ()
    {
        return zDepth;
    }

}
