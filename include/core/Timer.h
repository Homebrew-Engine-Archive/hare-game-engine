#ifndef _TIMER_H_
#define _TIMER_H_

#include "CorePrerequisites.h"

namespace hare
{
    CORE_API Timer& getTimer();
    CORE_API float getTime();

	class CORE_API Timer
	{
	public:
		Timer() : deltaTime(0.0f), FPS(0), frameCount(0)
		{
			curTime = getTime();
			oldFPSTime = curTime;
		}

		float getDeltaTime()
		{
			return deltaTime;
		}

		float getFPS()
		{
			return FPS;
		}

		void update();

	private:
		float deltaTime;
		float curTime;
		float FPS;
		uint32 frameCount;
		float oldFPSTime;
	};
}

#endif
