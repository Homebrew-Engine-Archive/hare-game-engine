#ifndef _GLRENDERWINDOW_H_
#define _GLRENDERWINDOW_H_

#include "../GLPrerequisites.h"

class GLRenderWindow : public RenderWindow
{
    friend class GLRenderSystem;
public:
    GLRenderWindow(bool);

    virtual ~GLRenderWindow();

    virtual void create(const WindowParams& params);

    virtual void resize(uint32 w, uint32 h);

    virtual void swapBuffer();

    virtual void destoryWindow();

    virtual void active();

    virtual void inactive();

private:
    void initalizeGLConfigParam();
	void createGLResource();
	void destoryGLResource();

    void setProjection();

private:
	uint32 displayMode;

	EGLDisplay dpy;
	EGLContext ctx;
	EGLSurface surface;
};


#endif
