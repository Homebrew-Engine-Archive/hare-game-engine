#ifndef _SCENELISTENER_H_
#define _SCENELISTENER_H_

#include "GraphicsPrerequisites.h"

namespace hare
{
	class GRAPHICS_API SceneListenerBase
	{
	public:
		SceneListenerBase();
		virtual ~SceneListenerBase();

		virtual void beginScene() = 0;
		virtual void renderScene() = 0;
		virtual void endScene() = 0;

	};
}

#endif
