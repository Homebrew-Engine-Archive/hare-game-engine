#include "PCH.h"
#include "Timer.h"

// Windows Platform
#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#	include <windows.h>
#	include <windowsx.h>
#	include <mmsystem.h>
#else
#   include <sys/time.h>
#endif

namespace hare
{
	float Timer::getTime()
	{
		float cur = 0.0f;

	#if HARE_PLATFORM == HARE_PLATFORM_WIN32
		static bool sInitialized = false;
		static bool sPrecisionTimerAvailable = false;
		static double sTimerSecondsConversion  = 0.001;

		if (!sInitialized)
		{
			sInitialized = true;

			LARGE_INTEGER freq;
			if (::QueryPerformanceFrequency(&freq))
			{
				sPrecisionTimerAvailable = true;
				sTimerSecondsConversion = 1.0 / freq.QuadPart;
			}
		}

		// query time
		LARGE_INTEGER counter;
		double seconds;
		if (sPrecisionTimerAvailable && ::QueryPerformanceCounter(&counter))
		{
			seconds = static_cast<double>(counter.QuadPart);
		}
		else
		{
			seconds = ::timeGetTime();
		}

		// convert the polled value to seconds
		cur = (float)(seconds * sTimerSecondsConversion);

	#else // HARE_PLATFORM == HARE_PLATFORM_WIN32

        static bool time_inited = false;
        static struct timeval start;
        if (!time_inited)
        {
            gettimeofday(&start, NULL);
            time_inited = true;
        }

        struct timeval now;
        gettimeofday(&now, NULL);
        cur = now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0f;

	#endif // HARE_PLATFORM == HARE_PLATFORM_WIN32

		static float last = 0.f;

		if (last >= cur)
			return last;
		else {
			last = cur;
			return cur;
		}
	}

	void Timer::update()
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

    Timer& getTimer()
    {
        static Timer timer;
        return timer;
    }

    float getTime()
    {
        return Timer::getTime();
    }
}

