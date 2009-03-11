#include "PCH.h"
#include "D3DPlugin.h"
#include "D3DSystemManager.h"

namespace hare_d3d
{
    class PluginRegistrant
    {
    public:
        PluginRegistrant()
        {
            systemManager = new D3DSystemManager;
            HareApp::getSingletonPtr()->setGraphicsSystem(systemManager);
        }
       ~PluginRegistrant()
       {
           SAFE_DELETE(systemManager);
       }

        D3DSystemManager* systemManager;
    };

    PluginRegistrant thePlugin;
}