#include "PCH.h"
#include "GLSystemManager.h"

GLSystemManager* systemManager;

void glplugin()
{
    systemManager = new GLSystemManager;
    HareApp::getSingletonPtr()->setGraphicsSystem(systemManager);
}


class PluginRegistrant
{
public:
	PluginRegistrant()
	{
	     getHareApp()->setPluginCallback(glplugin);
	}
	~PluginRegistrant()
	{
	}
};

PluginRegistrant thePlugin;
