#include "PCH.h"
#include "RenderWindow.h"

namespace hare_graphics
{
	RenderWindow::RenderWindow()
		:isMainWnd(false)
        ,isExternal(false)
		,sceneManager(NULL)
	{
	}

	RenderWindow::~RenderWindow()
	{
        sceneManager = NULL;
	}
}
