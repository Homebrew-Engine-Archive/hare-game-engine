#ifndef _COMPONENTSPRITE_H_
#define _COMPONENTSPRITE_H_

#include "GraphicsPrerequisites.h"

namespace hare
{
    class GRAPHICS_API ComponentSprite : public Sprite
    {
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
        void swapPart(int partID_1, int partID_2);
    
    protected:
        Sprite::List components;
    };
}


#endif