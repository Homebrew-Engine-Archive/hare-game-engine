#ifndef SCENELISTENER_H
#define SCENELISTENER_H

#include "graphics/Graphics.h"

bool notify_error(lua_State *L);

class LuaSceneListener : public SceneListenerBase
{
public:
    LuaSceneListener()
    {
		begin.L = 0;
		begin.ref = 0;
		render.L = 0;
		render.ref = 0;
		end.L = 0;
		end.ref = 0;
    }

    virtual ~LuaSceneListener()
    {
    }

    virtual void beginScene()
    {
		if (begin.L == 0)
			return ;

		swiglua_ref_get(&begin);

        if (lua_pcall(begin.L, 0, LUA_MULTRET, 0))
            notify_error(begin.L);
    }

    virtual void renderScene()
    {
		if (render.L == 0)
			return ;

		swiglua_ref_get(&render);

        if (lua_pcall(render.L, 0, LUA_MULTRET, 0))
            notify_error(render.L);
    }

    virtual void endScene()
    {
		if (end.L == 0)
			return ;

		swiglua_ref_get(&end);

        if (lua_pcall(end.L, 0, LUA_MULTRET, 0))
            notify_error(end.L);
    }

	void setBeginSceneListenFunc(SWIGLUA_REF fn)
	{
		begin = fn;
	}

	void setRenderSceneListenFunc(SWIGLUA_REF fn)
	{
		render = fn;
	}

	void setEndSceneListenFunc(SWIGLUA_REF fn)
	{
		end = fn;
	}

protected:
	SWIGLUA_REF begin;
	SWIGLUA_REF render;
	SWIGLUA_REF end;

};

#endif
