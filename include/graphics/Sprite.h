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

		virtual void beginScene() = 0;

		virtual void renderScene() = 0;

		virtual void endScene() = 0;
    
        void setOrigoPos(float x, float y);

        const PointF& getOrigoPos();

        void setFaceX(bool b);

        void setFaceY(bool b);

    protected:
        PointF origoPos;
        PointF pos;
        bool   bFaceX;
        bool   bFaceY;
    
    };
}

#endif
