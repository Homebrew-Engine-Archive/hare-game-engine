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

		virtual void beginScene();

		virtual void renderScene();

		virtual void endScene();

        int addPart(Sprite* s);

        bool removePart(int partID);

        bool removePart(Sprite* s);

        //NB: swap render sequence
        bool swapPart(int partID_1, int partID_2);

        Sprite* getPart(int partID);
    
    protected:
        Sprite::List::iterator getPartIT(int partID);

        Sprite::List components;
    };
}

#endif
