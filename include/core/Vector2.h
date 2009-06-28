#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include "CorePrerequisites.h"
#include "MathUtil.h"

namespace hare
{
    /** Standard 2-dimensional vector
    */
    class CORE_API Vector2
    {
    public:
        float x, y;

        Vector2(float _x, float _y)
        {
            x = _x;
            y = _y;
        }

        Vector2()
        {
            x = 0;
            y = 0;
        }

        Vector2	operator-() const
        {
            return Vector2(-x, -y);
        }

        Vector2	operator-(const Vector2 &v) const
        {
            return Vector2(x - v.x, y - v.y);
        }

        Vector2	operator+(const Vector2 &v) const
        {
            return Vector2(x + v.x, y + v.y);
        }

        Vector2& operator-=(const Vector2 &v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        Vector2& operator+=(const Vector2 &v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        bool operator==(const Vector2 &v) const
        {
            return (x == v.x && y == v.y);
        }

        bool operator!=(const Vector2 &v) const
        {
            return (x != v.x || y != v.y);
        }

        Vector2	operator/(const float scalar)	const
        {
            return Vector2( x / scalar, y / scalar);
        }

        Vector2	operator*(const float scalar) const
        {
            return Vector2(x * scalar, y * scalar);
        }

        Vector2& operator*=(const float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        
        /** Calculates the dot (scalar) product of this vector with another.
        */
        float dot(const Vector2 *v) const
        {
            return x*v->x + y*v->y;
        }

        /** Returns the length (magnitude) of the vector.
        */
        float length() const
        {
            return MathUtil::sqrtf(dot(this));
        }

        /** Calculates the angle between this vector and another.
        */
        float angle(const Vector2 *v = 0) const;

        /** Clamp the vector.
        */
        void clamp(const float max)
        {
            if (length() > max)
            {
                normalize();
                x *= max;
                y *= max;
            }
        }

        /** Normalize the vector.
        */
        Vector2* normalize()
        {
            float rc = MathUtil::fastInvsqrt(dot(this));
            x *= rc;
            y *= rc;
            return this;
        }

        /** Rotate the vector.
        */
        Vector2* rotate(float a);
    };

    inline Vector2 operator*(const float s, const Vector2 &v)
    {
        return v * s;
    }

    inline float operator^(const Vector2 &v, const Vector2 &u)
    {
        return v.angle(&u);
    }

    inline float operator%(const Vector2 &v, const Vector2 &u)
    {
        return v.dot(&u);
    }
}

#endif
