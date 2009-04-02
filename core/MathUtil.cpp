#include "PCH.h"
#include "MathUtil.h"
#include "Timer.h"

namespace hare_core
{
	const f32 MathUtil::PI = (4.0f * atan( 1.0f ));
	u32 MathUtil::g_seed = 0;

	void MathUtil::rand_seed(int seed)
	{
		if(!seed) g_seed = (u32)getTime();
		else g_seed = seed;
	}

	int MathUtil::rand_int(int min, int max)
	{
		g_seed = 214013 * g_seed + 2531011;
		if(max + 1 == min)
			max--;
		return min+(g_seed ^ g_seed>>15) % (max - min + 1);
	}

	f32 MathUtil::rand_float(f32 min, f32 max)
	{
		g_seed = 214013 * g_seed + 2531011;
		return min + (g_seed>>16) * (1.0f / 65535.0f) * (max - min);
	}
}