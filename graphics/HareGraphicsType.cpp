#include "PCH.h"
#include "HareGraphicsType.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_DYNAMIC_CLASS(Color, Object, 0)
	{
		HARE_META(R, f32)
		HARE_META(G, f32)
		HARE_META(B, f32)
		HARE_META(A, f32)
	}

}