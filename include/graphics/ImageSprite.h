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

        virtual void beginScene();

        virtual void renderScene();

        virtual void endScene();

        void loadFromImage(const String& filename);

        void setUV(float l, float r, float t, float b);

        void setUV(const RectF& rect);

        TextureMtrl* getTextureMtrl();
    
    protected:
        TextureMtrl::Ptr textureMtrl;
        RectF rectUV;

    };

}

#endif