#include "PCH.h"
#include "GLSystemManager.h"


class PluginRegistrant
{
public:
	PluginRegistrant()
	{
		systemManager = new GLSystemManager;
		HareApp::getSingletonPtr()->setGraphicsSystem(systemManager);
	}
	~PluginRegistrant()
	{
		SAFE_DELETE(systemManager);
	}

	GLSystemManager* systemManager;
};

PluginRegistrant thePlugin;