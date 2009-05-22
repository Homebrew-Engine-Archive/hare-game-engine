#include "../PCH.h"
#include "GLRenderWindow.h"
#include "../GLRenderSystem.h"
#include "../GLSystemManager.h"

#define COLOR_DEPTH 16

static int dblDepth[] = {GLX_RGBA, GLX_DEPTH_SIZE, COLOR_DEPTH, GLX_DOUBLEBUFFER, None};
static int dbl[]      = {GLX_RGBA, GLX_DOUBLEBUFFER, None};

GLXContext GLRenderWindow::main_context = None;

GLRenderWindow::GLRenderWindow(bool bMainWindow)
    :glContext(None)
	,oldMode(-1)
{
    //assert(isMainWnd && bMainWindow);
    isMainWnd = bMainWindow;

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

        int screen = DefaultScreen(dpy);
		Window rootWindow = RootWindow(dpy,screen);
		int left = (DisplayWidth(dpy, screen) - windowParams.width)/2;
		int top = (DisplayHeight(dpy, screen) - windowParams.height)/2;

        int dummy;

#ifndef NO_XRANDR
		// Attempt mode switch for fullscreen -- only if RANDR extension is there

		if(windowParams.bFullScreen && ! XQueryExtension(dpy, "RANDR", &dummy, &dummy, &dummy)) {
		    //log:can't switch to full screen mode: No XRANDR extension found
		    windowParams.bFullScreen = false;
		}else if(windowParams.bFullScreen) {
			// Use Xrandr extension to switch video modes. This is much better than
			// XVidMode as you can't scroll away from the full-screen applications.
			XRRScreenConfiguration *config;
			XRRScreenSize *sizes;
			Rotation current_rotation;
			int nsizes;

			// Get current screen info
			config = XRRGetScreenInfo(dpy, rootWindow);
			// Get available sizes
			if(config)
				sizes = XRRConfigSizes (config, &nsizes);

			if(config && nsizes > 0) {
                // Get current size and rotation
				oldMode = XRRConfigCurrentConfiguration (config, &current_rotation);
				// Find smallest matching mode
                int mode = -1;
                int mode_width = INT_MAX;
                int mode_height = INT_MAX;
				for(size_t i = 0; i < nsizes; i++){
                    if(sizes[i].width >= static_cast<int>(windowParams.width) &&
                        sizes[i].height >= static_cast<int>(windowParams.height) &&
                        sizes[i].width < mode_width &&
                        sizes[i].height < mode_height){

						mode = i;
						mode_width = sizes[i].width;
						mode_height = sizes[i].height;
					}
				}
				if(mode >= 0){
					// Finally, set the screen configuration
					XRRSetScreenConfig(dpy, config, rootWindow, mode, current_rotation, CurrentTime);
				}else{
					//log:Could not switch to full screen mode: No conforming mode was found
				    windowParams.bFullScreen = false;
				}
				// Free configuration data
				XRRFreeScreenConfigInfo(config);
			}else{
                //log:Could not switch to full screen mode: XRRGetScreenInfo failed
                windowParams.bFullScreen = false;
			}
        }
#endif

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

        //if(vi->class != TrueColor)
			//HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "TrueColor visual required for this program!", "GLRenderWindow::create");

        Colormap cmap;
		XSetWindowAttributes swa;
		unsigned long mask;
		cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.background_pixel = 0;
		swa.border_pixel = 0;
		swa.event_mask = StructureNotifyMask | VisibilityChangeMask;

		if(windowParams.bFullScreen) {
			mask = CWBackPixel | CWColormap | CWOverrideRedirect | CWSaveUnder | CWBackingStore | CWEventMask;
			swa.override_redirect = True;
			swa.backing_store = NotUseful;
			swa.save_under = False;
			// Fullscreen windows are always in the top left origin
			left = top = 0;
		}else{
		    mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
        }


		Window win;
		win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), left, top,
            windowParams.width, windowParams.height, 0, vi->depth, InputOutput, vi->visual,
            mask, &swa);


        XWMHints *wm_hints;
		if ((wm_hints = XAllocWMHints()) != NULL) {
			wm_hints->initial_state = NormalState;
			wm_hints->input = True;
			wm_hints->flags = StateHint | InputHint;
		}

		// Set size and location hints
		XSizeHints *size_hints;
		if ((size_hints = XAllocSizeHints()) != NULL) {
			// Otherwise some window managers ignore our position request
			size_hints->flags = USPosition;
		}

		// Make text property from title
		XTextProperty titleprop;
		char *lst = (char*)windowParams.title.c_str();
		XStringListToTextProperty((char **)&lst, 1, &titleprop);

		XSetWMProperties(dpy, win, &titleprop, NULL, NULL, 0, size_hints, wm_hints, NULL);

		// We don't like memory leaks. Free the clientside storage, but not the
		// pixmaps as they're still being used by the server.
		XFree(titleprop.value);
		XFree(wm_hints);
		XFree(size_hints);



		XSetStandardProperties(dpy, win, windowParams.title.c_str(), NULL, None,
			NULL, 0, NULL);

        XFlush(dpy);

        atomDeleteWindow = XInternAtom(dpy,"WM_DELETE_WINDOW",False);
		XSetWMProtocols(dpy, win, &atomDeleteWindow, 1);

	    XMapWindow(dpy, win);

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
    glXSwapBuffers((Display*)windowParams.hwnd.dpy, windowParams.hwnd.win);

    GLRenderSystem::getSingletonPtr()->clear(windowParams.bZbuffer);
}

void GLRenderWindow::destoryWindow()
{
    sceneManager = NULL;

    if (getIsMainWnd()){
        //if (!isExternal){
        getHareApp()->end();
        //}
    }else{
        //destory window
        GLSystemManager::getSingletonPtr()->destoryRenderWindow(this);
    }
    //NOTEC!! can't modify member value affter this funcation
}

void GLRenderWindow::active()
{
    glXMakeCurrent((Display*)windowParams.hwnd.dpy, None, NULL);

    glXMakeCurrent((Display*)windowParams.hwnd.dpy, windowParams.hwnd.win, glContext);

    setProjection();

	GLRenderSystem::getSingletonPtr()->setCurRenderWindow(this);
}

void GLRenderWindow::inactive()
{

}

void GLRenderWindow::createGLResource()
{
	glContext = glXCreateContext((Display*)windowParams.hwnd.dpy,
        (XVisualInfo*)windowParams.hwnd.vi, main_context,
		/* direct rendering if possible */ GL_TRUE);

	if (main_context == NULL){
        main_context = glContext;
	}

	active();
	if (isMainWnd){
	    glewInit();
	}

	(static_cast<GLRenderSystem*>(GLRenderSystem::getSingletonPtr()))->initalizeParam(windowParams.bZbuffer);
}

void GLRenderWindow::destoryGLResource()
{
	if(glContext){
	    glXDestroyContext((Display*)windowParams.hwnd.dpy, glContext);
	    glContext = 0;
    }


	if (!isExternal){
		if (windowParams.hwnd.vi){
		    XFree(windowParams.hwnd.vi);
		    windowParams.hwnd.vi = 0;
        }


		if(windowParams.hwnd.win){
		    XUnmapWindow((Display*)windowParams.hwnd.dpy, windowParams.hwnd.win);
		    XDestroyWindow((Display*)windowParams.hwnd.dpy, windowParams.hwnd.win);
		    windowParams.hwnd.win = 0;
	    }
    }


#ifndef NO_XRANDR
	if(windowParams.bFullScreen){
		// Restore original video mode.
		Window rootWindow = DefaultRootWindow((Display*)windowParams.hwnd.dpy);
		XRRScreenConfiguration *config;

		// Get current screen info
		config = XRRGetScreenInfo((Display*)windowParams.hwnd.dpy, rootWindow);
		if(config){
			Rotation current_rotation;
			XRRConfigCurrentConfiguration (config, &current_rotation);
			//Leaving full screen mode
			XRRSetScreenConfig((Display*)windowParams.hwnd.dpy, config, rootWindow, oldMode, current_rotation, CurrentTime);
			XRRFreeScreenConfigInfo(config);
        }else{
            //Could not switch from full screen mode: XRRGetScreenInfo failed
        }
    }
#endif
}

void GLXProc(const XEvent &event, GLRenderWindow* win)
{
    if(win == 0)
        return;

    if (event.xany.window != win->getWindowParams().hwnd.win )
        return;

    switch(event.type)
    {
    case ClientMessage:
        {
            if(event.xclient.format == 32 && event.xclient.data.l[0] == (long)(win->getAtomDeleteWindow()) ){
                //Window Closed (via X button)
			    //Send message first, to allow app chance to unregister things that need done before
			    //window is shutdown
			    win->destoryWindow();
		    }
        }
        break;
	case ConfigureNotify://Moving or Resizing
		{
			uint32 w = event.xconfigure.width > 0  ? event.xconfigure.width  : 1;
			uint32 h = event.xconfigure.height > 0 ? event.xconfigure.height : 1;
            win->resize(w, h);
		}
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
