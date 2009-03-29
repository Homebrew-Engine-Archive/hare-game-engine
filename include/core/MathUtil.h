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
        static bool isPO2(T n)
        {
            return (n & (n-1)) == 0;
        }


        static u32 firstPO2From(u32 n)
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

		static f32 rSin(f32 val)
		{
			return sin(val);
		}

		static f32 rCos(f32 val)
		{
			return cos(val);
		}

		template<class T>
		static T toRadian(T degree)
		{
			return (T)(degree * PI / 180.0);
		}

		static f32 rModf(f32 val, f32* intPart = 0)
		{
			f32 dummy; 
			if (!intPart) intPart = &dummy; 
			return modff(val, intPart); 
		}

	};
}

#endif