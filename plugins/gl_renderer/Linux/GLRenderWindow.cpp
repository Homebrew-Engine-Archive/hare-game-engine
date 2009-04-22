#include "PCH.h"
#include "GLRenderWindow.h"
#include "../GLRenderSystem.h"
#include "../GLSystemManager.h"

#define COLOR_DEPTH 16

static int dblDepth[] = {GLX_RGBA, GLX_DEPTH_SIZE, COLOR_DEPTH, GLX_DOUBLEBUFFER, None};
static int dbl[]      = {GLX_RGBA, GLX_DOUBLEBUFFER, None};

GLXContext GLRenderWindow::main_context = NULL;



GLRenderWindow::GLRenderWindow(bool bMainWindow)
    :glContext(NULL)
{
    assert(isMainWnd && bMainWindow);


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
    if (windowParams.hwnd.dpy == NULL){
		Display *dpy;
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't open a connection to the X server!", "GLRenderWindow::create"); 

		int dummy;
		if(!glXQueryExtension(dpy, &dummy, &dummy))
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "X server has no OpenGL GLX extension!", "GLRenderWindow::create"); 
		
		XVisualInfo *vi;
		if (windowParams.bZbuffer){
		    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblDepth);
		}else{
		    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dbl);
		}
		if (vi == NULL)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't find an OpenGL-capable RGBA visual!", "GLRenderWindow::create"); 

        if(vi->class != TrueColor)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "TrueColor visual required for this program!", "GLRenderWindow::create"); 

        Colormap cmap;
		XSetWindowAttributes swa;
		cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.border_pixel = 0;
		swa.event_mask = KeyPressMask | ExposureMask
            | ButtonPressMask | StructureNotifyMask;

		int screen = DefaultScreen(dpy);
		int left = DisplayWidth(dpy, screen);
		int top = DisplayHeight(dpy, screen);
		Window win;
		win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), left, top,
			windowParams.width, windowParams.height, 0, vi->depth, InputOutput, vi->visual,
			CWBorderPixel | CWColormap | CWEventMask, &swa);

		XSetStandardProperties(dpy, win, windowParams.title, NULL, None,
			NULL, 0, NULL);

	    XMapWindow(windowParams.hwnd.dpy, windowParams.hwnd.win);

		windowParams.hwnd.dpy = dpy;
		windowParams.hwnd.vi  = vi;
		windowParams.hwnd.win = win;

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

}

void GLRenderWindow::swapBuffer()
{
    glXSwapBuffers(windowParams.hwnd.dpy, windowParams.hwnd.win);

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
    glXMakeCurrent(windowParams.hwnd.dpy, windowParams.hwnd.win, glContext);

    setProjection();

	GLRenderSystem::getSingletonPtr()->setCurRenderWindow(this);
}

void GLRenderWindow::inactive()
{

}



void GLRenderWindow::createGLResource()
{
	glContext = glXCreateContext(windowParams.hwnd.dpy, windowParams.hwnd.vi, main_context,
		/* direct rendering if possible */ GL_TRUE);

	if (main_context == NULL){
        main_context = glContext;
	}

	glewInit();

	GLenum ret = glGetError();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}

void GLRenderWindow::destoryGLResource()
{
	if(glContext)
		glXDestroyContext(windowParams.hwnd.dpy, glContext);

	if (!isExternal){
		if (windowParams.hwnd.vi)
			XFree(windowParams.hwnd.vi);

		if(windowParams.hwnd.win)
			XDestroyWindow(windowParams.hwnd.dpy, windowParams.hwnd.win);	
	}
}

void GLXProc(const XEvent &event, GLRenderWindow* win)
{
	if( win == 0 ) return;

    switch(event.type)
    {
    case ClientMessage:

        break;
	case ConfigureNotify:	//Moving or Resizing

		break;
	case MapNotify:   //Restored

		break;
	case UnmapNotify: //Minimised

		break;
	case VisibilityNotify:
		switch(event.xvisibility.state)
		{
		case VisibilityUnobscured:

			break;
		case VisibilityPartiallyObscured:

			break;
		case VisibilityFullyObscured:

			break;
		}
		break;
	default:
		break;
	}
}