#ifndef _GLRENDERWINDOW_H_
#define _GLRENDERWINDOW_H_


class GLRenderWindow : public RenderWindow
{
    friend class GLRenderSystem;
public:
    GLRenderWindow(bool bMainWindow);

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
	static GLXContext main_context;
    GLXContext glContext;

    int oldMode;
};

void GLXProc(const XEvent &event, GLRenderWindow* win);

#endif
