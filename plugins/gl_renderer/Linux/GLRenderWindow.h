#ifndef _GLRENDERWINDOW_H_
#define _GLRENDERWINDOW_H_

class HareWidget : public Gtk::GL::DrawingArea
{
public:
	HareWidget(bool bZBuffer);
};

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
	static Glib::RefPtr<Gdk::GL::Context> main_context;
	static Glib::RefPtr<Gdk::GL::Window>  main_window;

	Gtk::Window *gtkWindow;
    HareWidget  *hareWidget;
protected:
	// Signal handlers
	bool on_delete_event(GdkEventAny* event);
	bool on_expose_event(GdkEventExpose* event);

};


#endif
