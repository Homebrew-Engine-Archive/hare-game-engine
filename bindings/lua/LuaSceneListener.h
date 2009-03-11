#ifndef SCENELISTENER_H
#define SCENELISTENER_H

#include "graphics/Graphics.h"

class LuaSceneListener : public SceneListenerBase
{
public:
    LuaSceneListener()
    {
		beginSceneListen.L = 0;
		beginSceneListen.ref = 0;
		renderSceneListen.L = 0;
		renderSceneListen.ref = 0;
		endSceneListen.L = 0;
		endSceneListen.ref = 0;
    }

    virtual ~LuaSceneListener()
    {
    }

    virtual void beginScene()
    {
		if (beginSceneListen.L == 0)
			return ;

		swiglua_ref_get(&beginSceneListen);

		lua_call(beginSceneListen.L,0,0); 
    }

    virtual void renderScene()
    {
		if (renderSceneListen.L == 0)
			return ;

		swiglua_ref_get(&renderSceneListen);

		lua_call(renderSceneListen.L,0,0); 
    }

    virtual void endScene()
    {
		if (endSceneListen.L == 0)
			return ;

		swiglua_ref_get(&endSceneListen);

		lua_call(endSceneListen.L,0,0); 
    }

	void setBeginSceneListenFun(SWIGLUA_REF fn)
	{
		beginSceneListen = fn;
	}

	void setRenderSceneListenFun(SWIGLUA_REF fn)
	{
		renderSceneListen = fn;
	}

	void setEndSceneListenFun(SWIGLUA_REF fn)
	{
		endSceneListen = fn;
	}

protected:
	SWIGLUA_REF beginSceneListen;
	SWIGLUA_REF renderSceneListen;
	SWIGLUA_REF endSceneListen;

};

#endif