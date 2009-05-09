#include "PCH.h"
#include "ImageSprite.h"
#include "Texture.h"
#include "Canvas.h"
#include "RenderSystem.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(ImageSprite, Sprite, 0)
    {
        HARE_META(rectUV, RectF)
        HARE_OBJ(textureMtrl, TextureMtrl)   
    }

    ImageSprite::ImageSprite()
        :rectUV(0, 0, 1, 1)
    {
        textureMtrl = new TextureMtrl;
    }

    ImageSprite::~ImageSprite()
    {

    }

    void ImageSprite::beginScene()
    {

    }

    void ImageSprite::renderScene()
    {
        RectF r = rectUV;
        int origoX = origoPos.x;
        int origoY = origoPos.y;
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
       
        getCanvas()->drawImage(pos.x + origoX, pos.y + origoY, textureMtrl, r);
    }

    void ImageSprite::endScene()
    {

    }

    void ImageSprite::loadFromImage(const String& filename)
    {
        Texture* tex;
        tex = RenderSystem::getSingletonPtr()->createTexture();
        tex->createFromFile(filename);

        textureMtrl->setTexture(tex);
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

    TextureMtrl* ImageSprite::getTextureMtrl()
    {
        return textureMtrl;
    }
}