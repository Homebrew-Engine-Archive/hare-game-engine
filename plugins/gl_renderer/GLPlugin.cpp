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
		if (!getHareApp())
			__pspgl_log("%s (%d): load gl plugin error", __FUNCTION__, __LINE__);
	    getHareApp()->setPluginCallback(glplugin);
	}
	~PluginRegistrant()
	{
	}
};

PluginRegistrant thePlugin;
