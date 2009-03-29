#ifndef _MATHUTIL_H_
#define _MATHUTIL_H_

#include "CorePrerequisites.h"

namespace hare_core
{
	class CORE_API MathUtil
	{
	public:
		static const f32 PI; 

		template<typename T>
        static inline bool isPO2(T n)
        {
            return (n & (n-1)) == 0;
        }

        static inline u32 firstPO2From(u32 n)
        {
            --n;            
            n |= n >> 16;
            n |= n >> 8;
            n |= n >> 4;
            n |= n >> 2;
            n |= n >> 1;
            ++n;
            return n;
        }

		static inline f32 sinf(f32 val)
		{
            return ::sinf(val);
		}

		static inline f32 cosf(f32 val)
		{
            return ::cosf(val);
		}

        static inline f32 acosf(f32 val)
        {
            return ::acosf(val);
        }

        static inline f32 atan2f(f32 x, f32 y)
        {
            return ::atan2f(x, y);
        }

        static inline f32 sqrtf(f32 val)
        {
            return ::sqrtf(val);
        }

		template<class T>
		static inline T toRadian(T degree)
		{
			return (T)(degree * PI / 180.0);
		}

		static inline f32 modff(f32 val, f32* intPart = 0)
		{
			f32 dummy; 
			if (!intPart) intPart = &dummy; 
            return ::modff(val, intPart); 
		}

        static inline f32 fastInvsqrt(f32 x)
        {
            union {
                s32 s32Part;
                f32 f32Part;
            } conv;

            conv.f32Part = x;
            conv.s32Part = 0x5f3759df - (conv.s32Part >> 1);
            return conv.f32Part * (1.5f - 0.4999f * x * conv.f32Part * conv.f32Part);
        }

	};
}

#endif