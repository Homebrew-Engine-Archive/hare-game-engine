#include "PCH.h"
#include "Sprite.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(Sprite, Object, 0)
    {
        HARE_META(origoPos, PointF)
    }

	Sprite::Sprite()
        :origoPos(0, 0)
        ,bFaceX(true)
        ,bFaceY(true)
	{

	}
	Sprite::~Sprite()
	{

	}

    void Sprite::setOrigoPos(float x, float y)
    {
        origoPos.x = x;origoPos.y = y;
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

    void Sprite::setFaceX(bool b)
    {
        bFaceX = b;
    }

    void Sprite::setFaceY(bool b)
    {
        bFaceY = b;
    }
}
