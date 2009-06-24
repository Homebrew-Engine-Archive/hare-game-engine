#include "PCH.h"
#include "MathUtil.h"
#include "Timer.h"

namespace hare
{
	const float MathUtil::PI = (4.0f * atan( 1.0f ));
	uint32 MathUtil::rand_seed = 0;

	void MathUtil::randSeed(uint32 seed)
	{
		if (!seed) rand_seed = (uint32)getTime();
		else rand_seed = seed;
	}

	int32 MathUtil::randRange(int32 min, int32 max)
	{
		rand_seed = 214013 * rand_seed + 2531011;
		if (max + 1 == min)
			max--;
		return min + (rand_seed ^ rand_seed >> 15) % (max - min + 1);
	}

	float MathUtil::randRange(float min, float max)
	{
		rand_seed = 214013 * rand_seed + 2531011;
		return min + (rand_seed >> 16) * (1.0f / 65535.0f) * (max - min);
	}
}
