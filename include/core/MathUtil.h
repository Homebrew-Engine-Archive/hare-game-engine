#ifndef _MATHUTIL_H_
#define _MATHUTIL_H_

#include "CorePrerequisites.h"

namespace hare_core
{
	class CORE_API MathUtil
	{
	public:
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
	};
}

#endif