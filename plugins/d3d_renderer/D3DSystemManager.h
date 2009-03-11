#ifndef D3DSYSTEMMANAGER
#define D3DSYSTEMMANAGER


namespace hare_d3d
{


	class D3DSystemManager : public SystemManager
	{
	public:
		D3DSystemManager();

		virtual ~D3DSystemManager();

		virtual RenderWindow* createRenderWindow(const WindowParams& params);

		virtual void destoryRenderWindow(RenderWindow* renderWindow);

		virtual Texture* createTexture();

		virtual void hareRun();

		virtual void hareRunFrame();

	};
}

#endif