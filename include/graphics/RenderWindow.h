#ifndef RENDERWINDOW
#define RENDERWINDOW


#include "GraphicsPrerequisites.h"
#include "RenderTarget.h"

namespace hare_graphics
{
	struct GRAPHICS_API WindowParams
    {
		uint32    hwnd;
		bool   bFullScreen;
		uint32    width;
		uint32    height;
		bool   bZbuffer;
		String title;
		WindowParams()
			:hwnd(0)
			,width(800)
			,height(600)
		{
			title = "hare";
		}
	};

	class SceneManager;

	class GRAPHICS_API RenderWindow : public RenderTarget
	{
	public:
		RenderWindow();

		virtual ~RenderWindow();

		uint32 getWidth()
		{
			return windowParams.width;
		}
		uint32 getHeight()
		{
			return windowParams.height;
		}

		virtual void create(const WindowParams& params) = 0;

		virtual void resize(uint32 w, uint32 h) = 0;

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

		const WindowParams& getWindowParams()
		{
			return windowParams;
		}

	protected:
		bool isMainWnd;
        bool isExternal;

		WindowParams windowParams;
		
		SceneManager* sceneManager;
	};
}

#endif
