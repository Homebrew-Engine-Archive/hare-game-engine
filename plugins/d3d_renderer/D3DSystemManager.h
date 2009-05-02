#ifndef _D3DSYSTEMMANAGER_H_
#define _D3DSYSTEMMANAGER_H_

class D3DSystemManager : public SystemManager
{
    HARE_DECLARE_DYNAMIC_CLASS(D3DSystemManager)

public:
	D3DSystemManager();

	virtual ~D3DSystemManager();

	virtual RenderWindow* createRenderWindow(const WindowParams& params);

	virtual void destoryRenderWindow(RenderWindow* renderWindow);

	virtual Texture* createTexture();

	virtual void hareRun();

	virtual int  hareRunFrame();

};


#endif