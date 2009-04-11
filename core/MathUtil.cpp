#include "PCH.h"
#include "MathUtil.h"
#include "Timer.h"

namespace hare_core
{
	const float MathUtil::PI = (4.0f * atan( 1.0f ));
	uint32 MathUtil::g_seed = 0;

	void MathUtil::rand_seed(int seed)
	{
		if(!seed) g_seed = (uint32)getTime();
		else g_seed = seed;
	}

	int MathUtil::rand_int(int min, int max)
	{
		g_seed = 214013 * g_seed + 2531011;
		if(max + 1 == min)
			max--;
		return min+(g_seed ^ g_seed>>15) % (max - min + 1);
	}

	float MathUtil::rand_float(float min, float max)
	{
		g_seed = 214013 * g_seed + 2531011;
		return min + (g_seed>>16) * (1.0f / 65535.0f) * (max - min);
	}
}
