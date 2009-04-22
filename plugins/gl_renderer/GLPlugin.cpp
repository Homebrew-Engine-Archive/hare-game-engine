#include "PCH.h"
#include "GLSystemManager.h"

GLSystemManager* systemManager;

class PluginRegistrant
{
public:
	PluginRegistrant()
	{
		systemManager = new GLSystemManager;
		HareApp::getSingletonPtr()->setGraphicsSystem(systemManager);
		printf("plugin : gl_renderer \n");
	}
	~PluginRegistrant()
	{
		delete systemManager;
	}
};

PluginRegistrant thePlugin;
