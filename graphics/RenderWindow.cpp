#include "PCH.h"
#include "RenderWindow.h"

namespace hare
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
