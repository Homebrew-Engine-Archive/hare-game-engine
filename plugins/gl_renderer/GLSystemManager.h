#ifndef _GLSYSTEMMANAGER_H_
#define _GLSYSTEMMANAGER_H_


class GLSystemManager : public SystemManager
{
    HARE_DECLARE_DYNAMIC_CLASS(GLSystemManager)

public:
	GLSystemManager();

	virtual ~GLSystemManager();

	virtual RenderWindow* createRenderWindow(const WindowParams& params);

	virtual void destoryRenderWindow(RenderWindow* renderWindow);

	virtual Texture* createTexture();

	virtual int hareRunFrame();

	virtual void hareRun();
};


#endif
