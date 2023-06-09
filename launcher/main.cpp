#include "core/Core.h"
#include "graphics/Graphics.h"
#include "gui/GUI.h"
using namespace hare;

bool loadPlugins()
{
    ConfigFile plugin;
    plugin.load("plugin.cfg");

    String pluginDir = plugin.getSetting("PluginDir");

    StringVector plugins = plugin.getMultiSetting("Plugin");
    for (size_t i = 0; i < plugins.size(); ++i)
    {
        String fileName = pluginDir + plugins[i];

        if (PluginManager::getSingletonPtr()->loadPlugin(fileName))
        {
            Log::getSingleton().logInfo("Plugin loaded : '%s'", fileName.c_str());
        }
        else
        {
            Log::getSingleton().logError("Failed to load plugin : '%s'", fileName.c_str());
        }
    }

    return true;
}

bool loadFileSystem()
{
    ConfigFile filesystem;
    filesystem.load("filesystem.cfg");

    String writeDir = filesystem.getSetting("WriteDir");
    String scriptDir = filesystem.getSetting("ScriptDir");
    StringVector searchPaths = filesystem.getMultiSetting("SearchPath");

    FileSystem* fs = FileSystem::getSingletonPtr();

    fs->setWriteDir(writeDir);
    Log::getSingleton().logInfo("Filesystem write dir : '%s'", writeDir.c_str());

    fs->addSearchPath(scriptDir);
    Log::getSingleton().logInfo("Filesystem append search path : '%s'", scriptDir.c_str());

    for (size_t i = 0; i < searchPaths.size(); ++i)
    {
        fs->addSearchPath(searchPaths[i]);
        Log::getSingleton().logInfo("Filesystem append search path : '%s'", searchPaths[i].c_str());
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
//  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    core_init(NULL);
    CmdLineParser cmdLine(cmd);
#else

int main(int argc, char *argv[])
{
    core_init(argv[0]);
    CmdLineParser cmdLine(argc, argv);
#endif

    loadPlugins();
    loadFileSystem();

    graphics_init();
    gui_init();

    String game = CmdLineParser::getSingleton().getOptionValue("game") + "/game_app.xml";

    GameApp::Ptr gameApp = NULL;

    try 
    {
        gameApp = (GameApp*)Object::importObject(game);
    }
    catch (...)
    {
    }

    if (gameApp && gameApp->isA(&GameApp::CLASS_INFO))
    {
        gameApp->go();
    }
    else
    {
        String err = "Failed to create GameApp from file, url : " + game;
        Log::getSingleton().logError(err.c_str());
    }

    gameApp = NULL;

    gui_quit();
    graphics_quit();
    core_quit();

    return EXIT_SUCCESS;
}
