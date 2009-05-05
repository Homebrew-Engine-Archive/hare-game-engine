#include "PCH.h"
#include "RenderWindow.h"

namespace hare
{
	RenderWindow::RenderWindow()
		:isMainWnd(false)
        ,isExternal(false)
		,sceneManager(NULL)
		,cameraPos(0, 0)
	{
	}

	RenderWindow::~RenderWindow()
	{
        sceneManager = NULL;
	}

	void RenderWindow::moveCamera(float dx, float dy)
	{
	    cameraPos.x += dx; cameraPos.y += dy;
	}

    void RenderWindow::moveCameraTo(float x, float y)
	{
		cameraPos.x = x; cameraPos.y = y;
	}

}
