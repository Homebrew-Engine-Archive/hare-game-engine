//***************************************************************
//  File:    ImageSprite.h
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
#ifndef _IMAGESPRITE_H_
#define _IMAGESPRITE_H_

#include "GraphicsPrerequisites.h"
#include "Material.h"
#include "Sprite.h"

namespace hare
{
    class GRAPHICS_API ImageSprite : public Sprite
    {
        HARE_DECLARE_DYNAMIC_CLASS(ImageSprite)
    public:
        ImageSprite();
        virtual ~ImageSprite();

        virtual void render();

        void loadFromImage(const String& filename);

        void loadFromMaterial(Material* m);

        void setUV(float l, float r, float t, float b);

        void setUV(const RectF& rect);

        Material* getMaterial();
    
    protected:
        Material::Ptr mtrl;
        RectF rectUV;

    };

}

#endif