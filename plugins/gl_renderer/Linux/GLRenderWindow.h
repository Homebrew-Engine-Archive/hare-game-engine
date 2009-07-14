#ifndef _GLRENDERWINDOW_H_
#define _GLRENDERWINDOW_H_


class GLRenderWindow : public RenderWindow
{
    friend class GLRenderSystem;
public:
    GLRenderWindow(bool isMainWindow);

    virtual ~GLRenderWindow();

    virtual void create(const WindowParams& params);

    virtual void resize(uint32 w, uint32 h);

    virtual void swapBuffer();

    virtual void destoryWindow();

    virtual void active();

    virtual void inactive();

    virtual bool getCustomData(const String& key, void* data);

    Display* getDisplay()
    {
        return display;
    }

    Window getWindow()
    {
        return window;
    }

    ::Atom getAtomDeleteWindow()
    {
        return atomDeleteWindow;
    }

private:
    void initalizeGLConfigParam();
    void createGLResource();
    void destoryGLResource();

    void setProjection();
private:
	static GLXContext main_context;
    GLXContext glContext;

    int oldMode;
    ::Atom atomDeleteWindow;
    Window window;
    Display* display;
    XVisualInfo* visualInfo;
};

void GLXProc(const XEvent &event, GLRenderWindow* win);

#endif
