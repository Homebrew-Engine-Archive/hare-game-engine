


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
    getHareApp()->end();

    return 0;
}

int power_callback(int unknown, int pwrflags, void *common)
{
    if ((pwrflags & (PSP_POWER_CB_POWER_SWITCH | PSP_POWER_CB_STANDBY)) > 0)
    {
        // TODO pause the game
        getHareApp()->pause();
    }
    else if ((pwrflags & PSP_POWER_CB_RESUME_COMPLETE) > 0)
    {
        sceKernelDelayThread(1500000);
        // TODO resume the game
        getHareApp()->resume();
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
    pspDebugScreenPrintf("%s\n", stream.c_str());

    return 0;
}

void psp_hook_lua_print(lua_State *L)
{
    lua_pushcfunction(L, hook_lua_print);
    lua_setglobal(L, "print");
}

#endif // HARE_PLATFORM == HARE_PLATFORM_PSP
