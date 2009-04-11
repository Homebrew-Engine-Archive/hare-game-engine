#ifndef _TIMER_H_
#define _TIMER_H_

#include "CorePrerequisites.h"

namespace hare_core
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

		void update()
		{
			float cur = getTime();
			deltaTime = cur - curTime;
			curTime = cur;

			frameCount++;

			if (curTime - oldFPSTime > 1)
			{
				FPS = frameCount / (curTime - oldFPSTime);
				oldFPSTime = curTime;
				frameCount = 0;
			}
		}

	private:
		float deltaTime;
		float curTime;
		float FPS;
		uint32 frameCount;
		float oldFPSTime;
	};
}

#endif
