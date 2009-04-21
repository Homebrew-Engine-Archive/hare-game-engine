#include "PCH.h"
#include "GLRenderWindow.h"
#include "../GLRenderSystem.h"
#include "../GLSystemManager.h"

Glib::RefPtr<Gdk::GL::Context> GLRenderWindow::main_context = NULL;
Glib::RefPtr<Gdk::GL::Window>  GLRenderWindow::main_window  = NULL;

HareWidget::HareWidget(bool bZBuffer)
    :Gtk::GL::DrawingArea()
{
	Glib::RefPtr<Gdk::GL::Config> glconfig;

	Gdk::GL::ConfigMode mode = Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DOUBLE;
	if (bZBuffer)
		mode |= Gdk::GL::MODE_DEPTH;

	glconfig = Gdk::GL::Config::create(mode);
	if (glconfig.is_null()){
		//log 
	}

	// Inherit GL context from Ogre main context
	set_gl_capability(glconfig, GLRenderWindow::main_context);

	add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);

	if (!GLRenderWindow::main_context){
		GLRenderWindow::main_context = get_gl_context();
	}

	if (!GLRenderWindow::main_window){
		GLRenderWindow::main_window  = get_gl_window();
	}
}


GLRenderWindow::GLRenderWindow(bool bMainWindow)
    :gtkWindow(NULL)
    ,hareWidget(NULL)
{
    assert(isMainWnd && bMainWindow);

    static Gtk::Main kit(0, NULL);
    isMainWnd = bMainWindow;
	if (isMainWnd){
        Gtk::GL::init(0, NULL);
	}
}

GLRenderWindow::~GLRenderWindow()
{
    destoryGLResource();
}

void GLRenderWindow::initalizeGLConfigParam()
{

}

void GLRenderWindow::create(const WindowParams& params)
{
	windowParams = params;
    if (windowParams.hwnd == NULL){
		gtkWindow = new Gtk::Window();

		gtkWindow->set_title(windowParams.title);

		if (windowParams.bFullScreen){
			gtkWindow->set_decorated(false);//what's this?
			gtkWindow->fullscreen();
		}else{
			gtkWindow->set_default_size(windowParams.width, windowParams.height);
			int left = (gdk_screen_width()  - windowParams.width ) / 2;
			int top  = (gdk_screen_height() - windowParams.height) / 2;
			gtkWindow->move(left, top); 
		}

        isExternal = false;
    }else{
        isExternal = true;
    }

    //get information from config
    initalizeGLConfigParam();

    createGLResource();
}

void GLRenderWindow::setProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //NOTEC!! glOrtho funcation last tow args is used as negative
    //left right bottom top                                                 near far
    glOrtho(0, (GLfloat)windowParams.width, (GLfloat)windowParams.height, 0, -1.0, 1.0); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

    glViewport (0, 0, windowParams.width, windowParams.height);
}

void GLRenderWindow::resize(uint32 w, uint32 h)
{
    windowParams.width = w;
    windowParams.height= h;

    setProjection();

	if (gtkWindow)
		gtkWindow->resize(w, h);
}

void GLRenderWindow::swapBuffer()
{
	hareWidget->get_gl_window()->swap_buffers();

    GLRenderSystem::getSingletonPtr()->clear(windowParams.bZbuffer);
}

void GLRenderWindow::destoryWindow()
{
    sceneManager = NULL;

    if (getIsMainWnd()){
        if (!isExternal){

        }
    }else{
        //destory window 
        GLSystemManager::getSingletonPtr()->destoryRenderWindow(this);	
    }
    //NOTEC!! can't modify member value affter this funcation
}

void GLRenderWindow::active()
{
    hareWidget->get_gl_window()->gl_begin(hareWidget->get_gl_context());

    setProjection();

	GLRenderSystem::getSingletonPtr()->setCurRenderWindow(this);
}

void GLRenderWindow::inactive()
{
	//allright? I don't know.
    hareWidget->get_gl_window()->gl_end();
}



void GLRenderWindow::createGLResource()
{
    hareWidget = Gtk::manage(new HareWidget(windowParams.bZbuffer));

	hareWidget->set_size_request(windowParams.width, windowParams.height);

	hareWidget->signal_delete_event().connect(SigC::slot(*this, &GLRenderWindow::on_delete_event));
	hareWidget->signal_expose_event().connect(SigC::slot(*this, &GLRenderWindow::on_expose_event));

	if(gtkWindow) {
		gtkWindow->add(*hareWidget);
		gtkWindow->show_all();
	}

	if(windowParams.hwnd) {
		// Attach it!
		// Note that the parent widget *must* be visible already at this point,
		// or the widget won't get realized in time for the GLinit that follows
		// this call. This is usually the case for Glade generated windows, anyway.
		windowParams.hwnd->add(*hareWidget);
		hareWidget->show();
	}

	glewInit();

	GLenum ret = glGetError();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}

void GLRenderWindow::destoryGLResource()
{
	if (!gtkWindow){
		delete gtkWindow;
		gtkWindow = NULL;
	}
}

bool GLRenderWindow::on_delete_event(GdkEventAny* event)
{
	destoryWindow();
    return false;
}

bool GLRenderWindow::on_expose_event(GdkEventExpose* event)
{
	//maybe call this funcation
	//swapBuffer();
	return false;
}


