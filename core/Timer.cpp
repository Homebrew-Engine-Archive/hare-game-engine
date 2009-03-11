#include "PCH.h"
#include "Timer.h"

// Windows Platform
#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#	include "windows.h"
#	include <windowsx.h>
#	include "mmsystem.h"
#	pragma comment ( lib, "winmm" )
#endif

namespace hare_core
{
	f32 getTime()
	{
		f32 cur;

	#if HARE_PLATFORM == HARE_PLATFORM_WIN32
		// defaults are for timeGetTime()
		
		static bool sInitialized             = false;
		static bool sPrecisionTimerAvailable = false;
		static double sTimerSecondsConversion  = 0.001;		// 1 msec
		
		// get multipliers
		if ( !sInitialized )
		{
			sInitialized = true;
			
			LARGE_INTEGER freq;
			if ( ::QueryPerformanceFrequency( &freq ) )
			{
				sPrecisionTimerAvailable = true;
				sTimerSecondsConversion = 1.0 / freq.QuadPart;
			}
		}
		
		// query time
		LARGE_INTEGER counter;
		double seconds;
		if ( sPrecisionTimerAvailable && ::QueryPerformanceCounter( &counter ) )
		{
			seconds = static_cast<double> ( counter.QuadPart);
		}
		else
		{
			seconds = ::timeGetTime();
		}
		
		// convert the polled value to seconds
		cur = (f32)( seconds * sTimerSecondsConversion );

	#else // #if OO_PLATFORM == OO_PLATFORM_WIN32
		
		cur = (f32)clock() / CLOCKS_PER_SEC;

	#endif

		// avoid the case of Utility::GetTime() returning the time which may smaller the last one, really weired.
		static f32 last = 0.f;

		if (last >= cur)
			return last;
		else {
			last = cur;
			return cur;
		}
	}

	Timer& getTimer()
	{
		static Timer timer;
		return timer;
	}
}

