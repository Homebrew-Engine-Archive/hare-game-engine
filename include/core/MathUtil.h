#ifndef _MATHUTIL_H_
#define _MATHUTIL_H_

#include "CorePrerequisites.h"

namespace hare_core
{
	class CORE_API MathUtil
	{
	public:
		static const float PI;

		template<typename T>
        static inline bool isPO2(T n)
        {
            return (n & (n-1)) == 0;
        }

        static inline uint32 firstPO2From(uint32 n)
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

		static inline float sinf(float val)
		{
            return ::sinf(val);
		}

		static inline float cosf(float val)
		{
            return ::cosf(val);
		}

        static inline float acosf(float val)
        {
            return ::acosf(val);
        }

        static inline float atan2f(float x, float y)
        {
            return ::atan2f(x, y);
        }

        static inline float sqrtf(float val)
        {
            return ::sqrtf(val);
        }

		template<class T>
		static inline T toRadian(T degree)
		{
			return (T)(degree * PI / 180.0);
		}

		static inline float modff(float val, float* intPart = 0)
		{
			float dummy;
			if (!intPart) intPart = &dummy;
            return ::modff(val, intPart);
		}

        static inline float fastInvsqrt(float x)
        {
            union {
                int32 s32Part;
                float f32Part;
            } conv;

            conv.f32Part = x;
            conv.s32Part = 0x5f3759df - (conv.s32Part >> 1);
            return conv.f32Part * (1.5f - 0.4999f * x * conv.f32Part * conv.f32Part);
        }

		static uint32 g_seed;

		static void rand_seed(int seed);

		static int rand_int(int min, int max);

		static float rand_float(float min, float max);

		template <typename T, typename U> inline
		static bool clampMin( T& val, const U& min_val )
		{
			if ( val < min_val )
			{
				val = min_val;
				return ( true );
			}
			return ( false );
		}

		template <typename T, typename U> inline
		static bool clampMax( T& val, const U& max_val )
		{
			if ( val > max_val )
			{
				val = max_val;
				return ( true );
			}
			return ( false );
		}

		template <typename T, typename U, typename V> inline
		static bool clampMinMax( T& val, const U& min_val, const V& max_val )
		{
			if ( val < min_val )
			{
				val = min_val;
				return ( true );
			}
			else if ( val > max_val )
			{
				val = max_val;
				return ( true );
			}
			return ( false );
		}
	};
}

#endif
