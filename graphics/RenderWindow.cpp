#include "PCH.h"
#include "RenderWindow.h"

namespace hare_graphics
{
	RenderWindow::RenderWindow()
		:isMainWnd(false)
		,width(0)
		,height(0)
		,sceneManager(NULL)
	{
	}

	RenderWindow::~RenderWindow()
	{
	}
}