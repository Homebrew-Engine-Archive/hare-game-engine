#ifndef _TIMER_H_
#define _TIMER_H_

#include "CorePrerequisites.h"

namespace hare_core
{
    CORE_API Timer& getTimer();
    CORE_API f32 getTime();

	class CORE_API Timer
	{
	public:
		Timer() : deltaTime(0.0f), FPS(0), frameCount(0)
		{
			curTime = getTime();
			oldFPSTime = curTime;
		}

		f32 getDeltaTime()
		{
			return deltaTime;
		}

		f32 getFPS()
		{
			return FPS;
		}

		void update()
		{
			f32 cur = getTime();
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
		f32 deltaTime;
		f32 curTime;
		f32 FPS;
		u32 frameCount;
		f32 oldFPSTime;
	};
}

#endif
