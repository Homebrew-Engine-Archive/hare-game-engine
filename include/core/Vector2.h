#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include "CorePrerequisites.h"

namespace hare_core
{
    class CORE_API Vector2
    {
    public:
        f32	x, y;

        Vector2(f32 _x, f32 _y)	
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

        Vector2	operator/(const f32 scalar)	const 
        { 
            return Vector2( x / scalar, y / scalar); 
        }
        
        Vector2	operator*(const f32 scalar) const 
        { 
            return Vector2(x * scalar, y * scalar); 
        }
        
        Vector2& operator*=(const f32 scalar)
        { 
            x *= scalar; 
            y *= scalar; 
            return *this;  
        }

        f32	dot(const Vector2 *v) const 
        { 
            return x*v->x + y*v->y; 
        }

        f32	length() const 
        { 
            return MathUtil::sqrtf(dot(this)); 
        }
        
        f32	angle(const Vector2 *v = 0) const;

        void clamp(const f32 max) 
        { 
            if (length() > max)	
            { 
                normalize(); 
                x *= max; 
                y *= max; 
            } 
        }
        
        Vector2* normalize() 
        { 
            f32 rc = MathUtil::fastInvsqrt(dot(this)); 
            x *= rc; 
            y *= rc; 
            return this; 
        }
        
        Vector2* rotate(f32 a);
    };

    inline Vector2 operator*(const f32 s, const Vector2 &v)
    { 
        return v * s; 
    }
    
    inline f32 operator^(const Vector2 &v, const Vector2 &u)
    { 
        return v.angle(&u); 
    }
    
    inline f32 operator%(const Vector2 &v, const Vector2 &u) 
    { 
        return v.dot(&u); 
    }
}

#endif