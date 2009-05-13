//***************************************************************
//  File:    ImageSprite.cpp
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
#include "ImageSprite.h"
#include "Texture.h"
#include "Canvas.h"
#include "RenderSystem.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(ImageSprite, Sprite, 0)
    {
        HARE_META_F(rectUV, RectF, propHide)
        HARE_OBJ(mtrl, Material)   
    }

    ImageSprite::ImageSprite()
        :rectUV(0, 0, 1, 1)
    {
    }

    ImageSprite::~ImageSprite()
    {

    }

    void ImageSprite::beginScene()
    {

    }

    void ImageSprite::renderScene()
    {
        if (!mtrl)
            return;

        TextureMtrl* textureMtrl = mtrl->getTextureMtrl();
        if (!textureMtrl)
            return;

        Texture* texture = textureMtrl->getTexture();

        RectF r = rectUV;
        RectF drawRect;
        drawRect.minX = 0;
        drawRect.minY = 0;
        drawRect.maxX = (rectUV.maxX - rectUV.minX) * texture->getWidth();
        drawRect.maxY = (rectUV.maxY - rectUV.minY) * texture->getHeight();

        float origoX = origoPos.x;
        float origoY = origoPos.y;
        if (!bFaceX){
            r.minX = rectUV.maxX;
            r.maxX = rectUV.minX;
            origoX = -origoX;
        }
        if (!bFaceY){
            r.minY = rectUV.maxY;
            r.maxY = rectUV.minY;
            origoY = -origoY;
        }

        drawRect.moveTo(pos.x + origoX, pos.y + origoY);
       
        getCanvas()->drawImage(drawRect, mtrl, r);
    }

    void ImageSprite::endScene()
    {

    }

    void ImageSprite::loadFromImage(const String& filename)
    {
        Texture* tex;
        tex = RenderSystem::getSingletonPtr()->createTexture();
        tex->createFromFile(filename);
        TextureMtrl* textureMtrl = new TextureMtrl;
        textureMtrl->setTexture(tex);
        mtrl = textureMtrl;
    }

    void ImageSprite::loadFromMaterial(Material* m)
    {
        mtrl = m;
    }

    void ImageSprite::setUV(float l, float r, float t, float b)
    {
        rectUV.minX = l;
        rectUV.maxX = r;
        rectUV.minY = t;
        rectUV.maxY = b;
    }

    void ImageSprite::setUV(const RectF& rect)
    {
        rectUV = rect;
    }

    Material* ImageSprite::getMaterial()
    {
        return mtrl;
    }
}