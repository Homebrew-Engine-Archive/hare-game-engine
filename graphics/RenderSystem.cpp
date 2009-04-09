#include "PCH.h"
#include "RenderSystem.h"
#include "RenderWindow.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_SINGLETON(RenderSystem)

	RenderSystem::RenderSystem()
		:curRenderTarget(NULL)
		,curRenderWindow(NULL)
	{

	}

	RenderSystem::~RenderSystem()
	{

	}

	void RenderSystem::setRenderTarget(RenderTarget* target)
	{
		if (curRenderTarget){
			curRenderTarget->inactive();
		}

		curRenderTarget = target;
		
		if (curRenderTarget){
			curRenderTarget->active();
		}
	}

	void RenderSystem::setCurRenderWindow(RenderWindow* window)
	{
		curRenderWindow = window;
		curRenderTarget = window;
	}

	RenderTarget* RenderSystem::getRenderTarget()
	{
		return curRenderTarget;
	}

	RenderWindow* RenderSystem::getCurRenderWindow()
	{
		return curRenderWindow;
	}
}
