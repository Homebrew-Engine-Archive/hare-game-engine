#ifndef RENDERWINDOW
#define RENDERWINDOW


#include "GraphicsPrerequisites.h"
#include "RenderTarget.h"

namespace hare_graphics
{
	struct GRAPHICS_API WindowParams{
		u32    hwnd;
		bool   bFullScreen;
		u32    width;
		u32    height;
		bool   bZbuffer;
		String title;
		WindowParams()
			:hwnd(0)
			,width(800)
			,height(600)
		{
			title = "gg";
		}
	};

	class SceneManager;

	class GRAPHICS_API RenderWindow : public RenderTarget
	{
	public:
		RenderWindow();

		virtual ~RenderWindow();

		void setWidth(u32 w)
		{
			width = w;
		}
		void setHeight(u32 h)
		{
			height = h;
		}
		u32 getWidth()
		{
			return width;
		}
		u32 getHeight()
		{
			return height;
		}

		virtual void create(const WindowParams& params) = 0;

		virtual void resize(u32 w, u32 h) = 0;

		virtual void swapBuffer() = 0;

		virtual void destoryWindow() = 0;

		void setSceneManager(SceneManager* scene)
		{
			sceneManager = scene;
		}

		SceneManager* getSceneManager()
		{
			return sceneManager;
		}
		
		bool getIsMainWnd()
		{
			return isMainWnd;
		}



	protected:
		u32 width;
		u32 height;
		bool isMainWnd;

		SceneManager* sceneManager;
	};
}

#endif