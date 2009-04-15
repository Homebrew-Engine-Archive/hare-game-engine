#include "core/Core.h"
#include "graphics/Graphics.h"
using namespace hare;

#include "LuaDebuggee.h"

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   if defined(_DEBUG)
#	    define  CRTDBG_MAP_ALLOC
#	    include <stdlib.h>
#	    include <crtdbg.h>
#   endif
#endif

#if HARE_PLATFORM == HARE_PLATFORM_PSP
#   include <pspkernel.h>
#   include <pspdebug.h>
#   include <pspdisplay.h>
#   include <pspctrl.h>
#   include <pspsdk.h>
#   include <psputility.h>
#   include <psppower.h>

/* Define the module info section */
PSP_MODULE_INFO("hare", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define printf	pspDebugScreenPrintf

int exit_callback(int arg1, int arg2, void *common)
{
    // TODO end the game
	sceKernelExitGame();

	return 0;
}

int power_callback(int unknown, int pwrflags, void *common)
{
	if ((pwrflags & (PSP_POWER_CB_POWER_SWITCH | PSP_POWER_CB_STANDBY)) > 0)
	{
	    // TODO pause the game
	}
	else if ((pwrflags & PSP_POWER_CB_RESUME_COMPLETE) > 0)
	{
		sceKernelDelayThread(1500000);
        // TODO resume the game
	}
	return 0;
}

int callback_thread(SceSize args, void *argp)
{
    int cbid;

	cbid = sceKernelCreateCallback("callback_thread", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);

	cbid = sceKernelCreateCallback("callback_thread", power_callback, NULL);
    scePowerRegisterCallback(0, cbid);

	sceKernelSleepThreadCB();

	return 0;
}

void psp_init()
{
	int thid = sceKernelCreateThread("callback_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
    }
}

void psp_quit()
{
    sceKernelExitGame();
}

static int hook_lua_print(lua_State *L)
{
    int idx;
    String stream;
    int n = lua_gettop(L);  /* number of arguments */
    lua_getglobal(L, "tostring");
    for (idx = 1; idx <= n;  idx++)
    {
        lua_pushvalue(L, -1);  /* function to be called */
        lua_pushvalue(L, idx);   /* value to print */
        lua_call(L, 1, 1);
        const char* s = lua_tostring(L, -1);  /* get result */
        if (!s)
            return luaL_error(L, "`tostring' must return a string to `print'");
        if (idx > 1)
            stream.append("\t");
        stream.append(s);
        lua_pop(L, 1);  /* pop result */
    }
    printf("%s\n", stream.c_str());

    return 0;
}

void psp_hook_lua_print(lua_State *L)
{
    lua_pushcfunction(L, hook_lua_print);
    lua_setglobal(L, "print");
}

#endif // HARE_PLATFORM == HARE_PLATFORM_PSP

extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>

    int luaopen_hare(lua_State *L);
}

int init_handle = -1;
int quit_handle = -1;

bool notify_error(lua_State *L)
{
    String err = luaL_checkstring(L, -1);

#if HARE_PLATFORM != HARE_PLATFORM_PSP
    if (LuaDebuggee::getSingletonPtr())
        return LuaDebuggee::getSingletonPtr()->notifyError(err + "\n");
    else
#endif
    {
        printf("%s\n", err.c_str());
        return false;
    }
}

void main_loop(lua_State *L)
{
    if (init_handle < 0 || quit_handle < 0)
        return;

    lua_rawgeti(L, LUA_REGISTRYINDEX, init_handle);
    int status = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (status)
        notify_error(L);

    getHareApp()->hareRun();

    lua_rawgeti(L, LUA_REGISTRYINDEX, quit_handle);
    status = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (status)
        notify_error(L);
}

bool load_scripts(const String& game, lua_State *L)
{
    FileSystem* fs = FileSystem::getSingletonPtr();
    String fileName = game + "/script.lua";
    FileHandle fh = fs->openFile(fileName, FM_Read);
    if (!fh)
        return false;
    int size = fs->size(fh);
    if (size <= 0)
    {
        fs->closeFile(fh);
        return false;
    }
    char* buffer = new char[size];
    fs->readFile(fh, buffer, size, 1);
    int status = luaL_loadbuffer(L, buffer, size, fileName.c_str());
    if (status)
        notify_error(L);
    else
    {
        status = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (status)
            notify_error(L);
    }

    delete [] buffer;

    lua_getglobal(L, "init");
    init_handle = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_getglobal(L, "quit");
    quit_handle = luaL_ref(L, LUA_REGISTRYINDEX);

    return true;
}

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmd, INT)
{
    core_init(NULL);
    CmdLineParser cmdLine(cmd);
#else // HARE_PLATFORM == HARE_PLATFORM_WIN32
int main(int argc, char *argv[])
{
    core_init(argv[0]);
    CmdLineParser cmdLine(argc, argv);
#endif // HARE_PLATFORM == HARE_PLATFORM_WIN32

#if HARE_PLATFORM == HARE_PLATFORM_PSP
    psp_init();
    pspDebugScreenInit();
#endif // HARE_PLATFORM == HARE_PLATFORM_PSP

    lua_State *L = lua_open();  /* create state */
    if (L == NULL) {
        return EXIT_FAILURE;
    }
	luaL_openlibs(L);

    luaopen_hare(L);

#if HARE_PLATFORM == HARE_PLATFORM_PSP
    psp_hook_lua_print(L);
#endif

    // load all plugins
    ConfigFile plugin;
    plugin.load("plugin.cfg");
    String pluginDir = plugin.getSetting("PluginDir");
    StringVector plugins = plugin.getMultiSetting("Plugin");
    for (size_t i = 0; i < plugins.size(); ++i)
    {
        getHareApp()->loadPlugin(pluginDir + plugins[i]);
    }

    // load all resources
    ConfigFile resource;
    resource.load("resource.cfg");

    String writeDir = resource.getSetting("WriteDir");
    String scriptDir = resource.getSetting("ScriptDir");
    StringVector searchPaths = resource.getMultiSetting("SearchPath");

    FileSystem* fs = FileSystem::getSingletonPtr();
    fs->setWriteDir(writeDir);
    fs->addSearchPath(scriptDir);
    for (size_t i = 0; i < searchPaths.size(); ++i)
    {
        fs->addSearchPath(searchPaths[i]);
    }

  	getHareApp()->startUp();

#if HARE_PLATFORM != HARE_PLATFORM_PSP
    LuaDebuggee* debuggee = 0;
    String debug = cmdLine.getOptionValue("debug");
    if (!debug.empty())
    {
        StringVector cmds = StringUtil::split(debug, ":");
        if (cmds.size() == 2)
        {
            String addr = cmds[0];
            int port = -1;
            StringConverter::parse(cmds[1], port);
            debuggee = new LuaDebuggee(L, addr, port);
        }
    }

    if (debuggee)
        debuggee->start();
#endif

    String game = cmdLine.getOptionValue("game");

    if (load_scripts(game, L))
    {
        main_loop(L);
    }

#if HARE_PLATFORM != HARE_PLATFORM_PSP
    if (debuggee)
    {
        debuggee->stop();
        delete debuggee;
        debuggee = 0;
    }
#endif
    lua_close(L);

	getHareApp()->shutDown();
	getHareApp()->freePlugin();

    core_quit();

#if HARE_PLATFORM == HARE_PLATFORM_PSP
    psp_quit();
#endif

	return EXIT_SUCCESS;
}
