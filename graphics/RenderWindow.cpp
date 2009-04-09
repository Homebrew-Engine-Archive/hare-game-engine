#include "PCH.h"
#include "RenderWindow.h"

namespace hare_graphics
{
	RenderWindow::RenderWindow()
		:isMainWnd(false)
		,sceneManager(NULL)
        ,isExternal(false)
	{
	}

	RenderWindow::~RenderWindow()
	{
        sceneManager = NULL;
	}
}
