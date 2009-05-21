#include "PCH.h"
#include "LuaGameApp.h"
#include "LuaDebuggee.h"

bool notify_error(lua_State *L)
{
    String err = luaL_checkstring(L, -1);

    Log::getSingleton().logError("Lua error : %s", err.c_str());

    if (LuaDebuggee::getSingletonPtr())
        return LuaDebuggee::getSingletonPtr()->notifyError(err + "\n");

    return false;
}

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   if defined(_DEBUG)
#	    define  CRTDBG_MAP_ALLOC
#	    include <stdlib.h>
#	    include <crtdbg.h>
#   endif

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmd, INT)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    LuaGameApp app(cmd);

#else

int main(int argc, char *argv[])
{
    LuaGameApp app(argc, argv);

#endif

    app.go();

    return EXIT_SUCCESS;
}
