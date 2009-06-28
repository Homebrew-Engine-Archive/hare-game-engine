#ifndef _TIMER_H_
#define _TIMER_H_

#include "CorePrerequisites.h"

namespace hare
{
    /** Timer class
    */
	class CORE_API Timer
	{
	public:
		Timer() : deltaTime(0.0f), FPS(0), frameCount(0)
		{
			curTime = getTime();
			oldFPSTime = curTime;
		}

        /// Get delta time between update
		float getDeltaTime()
		{
			return deltaTime;
		}

        /// Get FPS
		float getFPS()
		{
			return FPS;
		}

        /// Update the timer
		void update();

        /// Get current time in seconds since started
        static float getTime();

	private:
		float deltaTime;
		float curTime;
		float FPS;
		uint32 frameCount;
		float oldFPSTime;
	};

    CORE_API float getTime();

    CORE_API Timer& getTimer();
}

#endif
