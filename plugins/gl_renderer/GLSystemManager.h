#ifndef _GLSYSTEMMANAGER_H_
#define _GLSYSTEMMANAGER_H_

class GLSystemManager : public SystemManager
{
public:
	GLSystemManager();

	virtual ~GLSystemManager();

	virtual RenderWindow* createRenderWindow(const WindowParams& params);

	virtual void destoryRenderWindow(RenderWindow* renderWindow);

	virtual Texture* createTexture();

	virtual void hareRun();

	virtual int  hareRunFrame();
};


#endif
