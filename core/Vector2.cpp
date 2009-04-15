#include "PCH.h"
#include "Vector2.h"

namespace hare
{
    float Vector2::angle(const Vector2 *v) const
    {
        if (v)
        {
            Vector2 s = *this, t = *v;
            s.normalize();
            t.normalize();
            return MathUtil::acosf(s.dot(&t));
        }
        else
            return MathUtil::atan2f(y, x);
    }

    Vector2* Vector2::rotate(float a)
    {
        Vector2 v;

        v.x = x * MathUtil::cosf(a) - y * MathUtil::sinf(a);
        v.y = x * MathUtil::sinf(a) + y * MathUtil::cosf(a);

        x = v.x;
        y = v.y;

        return this;
    }
}
