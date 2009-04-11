#include "PCH.h"
#include "HareGraphicsType.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_DYNAMIC_CLASS(Color, Object, 0)
	{
		HARE_META(R, float)
		HARE_META(G, float)
		HARE_META(B, float)
		HARE_META(A, float)
	}
}
