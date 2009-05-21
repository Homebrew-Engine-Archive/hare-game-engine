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
        call(begin);
    }

    virtual void renderScene()
    {
        call(render);
    }

    virtual void endScene()
    {
        call(end);
    }

    void call(SWIGLUA_REF& func)
    {
        if (func.L == 0)
            return;

        swiglua_ref_get(&func);

        if (lua_pcall(func.L, 0, LUA_MULTRET, 0))
            notify_error(func.L);
    }

	void setBeginSceneCallback(SWIGLUA_REF func)
	{
		begin = func;
	}

	void setRenderSceneCallback(SWIGLUA_REF func)
	{
		render = func;
	}

	void setEndSceneCallback(SWIGLUA_REF func)
	{
		end = func;
	}

protected:
	SWIGLUA_REF begin;
	SWIGLUA_REF render;
	SWIGLUA_REF end;
};

#endif
