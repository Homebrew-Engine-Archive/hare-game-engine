#include "PCH.h"

bool loadPlugins()
{
    ConfigFile plugin;
    plugin.load("plugin.cfg");

    String pluginDir = plugin.getSetting("PluginDir");

    StringVector plugins = plugin.getMultiSetting("Plugin");
    for (size_t i = 0; i < plugins.size(); ++i)
    {
        String fileName = pluginDir + plugins[i];

        if (getHareApp()->loadPlugin(fileName))
        {
            Log::getSingleton().logInfo("Load plugin : '%s'", fileName.c_str());
        }
        else
        {
            Log::getSingleton().logError("HareApp::loadPlugin failed to load '%s'", fileName.c_str());
        }
    }

    return true;
}

bool loadResources()
{
    ConfigFile resource;
    resource.load("resource.cfg");

    String writeDir = resource.getSetting("WriteDir");
    String scriptDir = resource.getSetting("ScriptDir");
    StringVector searchPaths = resource.getMultiSetting("SearchPath");

    FileSystem* fs = FileSystem::getSingletonPtr();

    fs->setWriteDir(writeDir);
    Log::getSingleton().logInfo("Filesystem write dir : '%s'", writeDir.c_str());

    fs->addSearchPath(scriptDir);
    Log::getSingleton().logInfo("Filesystem add search path : '%s'", scriptDir.c_str());

    for (size_t i = 0; i < searchPaths.size(); ++i)
    {
        fs->addSearchPath(searchPaths[i]);
        Log::getSingleton().logInfo("Filesystem add search path : '%s'", searchPaths[i].c_str());
    }

    return true;
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
    //_CrtSetBreakAlloc(386);
    core_init(NULL);
    CmdLineParser cmdLine(cmd);
#else

int main(int argc, char *argv[])
{
    core_init(argv[0]);
    CmdLineParser cmdLine(argc, argv);
#endif

    loadPlugins();
    loadResources();

    graphics_init();
    gui_init();

    GameApp::Ptr app = NULL;

    ClassInfo* appClass = ClassInfo::findClass("LuaGameApp");

    if (appClass)
    {
        app = (GameApp*)appClass->createObject();
    }

    if (app)
    {
        app->go();
    }

    app = 0;

    gui_quit();
    graphics_quit();

    HareApp::getSingletonPtr()->freeAllPlugins();

    core_quit();

    return EXIT_SUCCESS;
}
