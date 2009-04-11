#ifndef _GLRENDERWINDOW_H_
#define _GLRENDERWINDOW_H_

class GLRenderWindow : public RenderWindow
{
    friend class GLRenderSystem;
public:
    GLRenderWindow(bool bMainWindow);

    virtual ~GLRenderWindow();

    virtual void create(const WindowParams& params);

    virtual void resize(u32 w, u32 h);

    virtual void swapBuffer();

    virtual void destoryWindow();

    virtual void active();

    virtual void inactive();

    HWND getWindowHandle();

    HGLRC getRenderContext();

    PIXELFORMATDESCRIPTOR* getPixelFormatDescriptor();

private:
    void initalizeGLConfigParam();
    void createGLResource();
    void destoryGLResource();

    void setProjection();
private:
    HGLRC hRC;
    HDC   hDC;
    PIXELFORMATDESCRIPTOR pfd;

};


#endif
