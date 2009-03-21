#include "PCH.h"
#include "RenderWindow.h"

namespace hare_graphics
{
	RenderWindow::RenderWindow()
		:isMainWnd(false)
		,width(0)
		,height(0)
		,sceneManager(NULL)
        ,isExternal(false)
	{
	}

	RenderWindow::~RenderWindow()
	{
        sceneManager = NULL;
	}
}