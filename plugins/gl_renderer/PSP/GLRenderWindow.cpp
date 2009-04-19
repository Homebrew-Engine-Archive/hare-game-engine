#include "GLRenderWindow.h"
#include "../GLRenderSystem.h"
#include "../GLSystemManager.h"






#define EGLCHK(x)                                      \
	do{                                               \
		EGLint errcode;                                \
		x;                                             \
		errcode = eglGetError();                       \
		if (errcode != EGL_SUCCESS) {                  \
		FLIE* fp = fopen("ms0:/log.txt"); \
			__FUNCTION__, __LINE__,                    \
			(unsigned int) errcode);                   \
        }                                              \
	}while(0);







#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 272

static EGLint attrib_list [] = {
    EGL_RED_SIZE,     8, /* 0 */
    EGL_GREEN_SIZE,   8, /* 2 */
    EGL_BLUE_SIZE,    8, /* 4 */
    EGL_ALPHA_SIZE,   0, /* 6 */
    EGL_STENCIL_SIZE, 0, /* 8 */
    EGL_DEPTH_SIZE,   0, /* 10 */
    EGL_NONE
};

GLRenderWindow::GLRenderWindow(bool)
    :dpy(NULL)
    ,ctx(NULL)
    ,surface(NULL)
{

}

GLRenderWindow::~GLRenderWindow()
{
    destoryGLResource();
}

void GLRenderWindow::initalizeGLConfigParam()
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}

void GLRenderWindow::create(const WindowParams& params)
{
	windowParams = params;
	windowParams.hwnd = NULL;
	windowParams.bFullScreen = true;
	windowParams.width = SCREEN_WIDTH;
	windowParams.height= SCREEN_HEIGHT;

	displayMode = GLUT_RGBA | GLUT_DOUBLE;
	if (windowParams.bZbuffer){
		displayMode |= GLUT_DEPTH;
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
	eglSwapBuffers(dpy, surface);
}

void GLRenderWindow::destoryWindow()
{
    sceneManager = NULL;
}

void GLRenderWindow::active()
{   
	GLRenderSystem::getSingletonPtr()->clear(true);

    setProjection();

	GLRenderSystem::getSingletonPtr()->setCurRenderWindow(this);
}

void GLRenderWindow::inactive()
{

}

void GLRenderWindow::createGLResource()
{
	EGLConfig config;
	EGLint num_configs;

	dpy = eglGetDisplay(0);
	eglInitialize(dpy, NULL, NULL);
	
    if (displayMode & GLUT_ALPHA)
        attrib_list[7]  = 8;
    if (displayMode & GLUT_STENCIL)
        attrib_list[9]  = 8;
    if (displayMode & GLUT_DEPTH)
        attrib_list[11] = 16;

	eglChooseConfig(dpy, attrib_list, &config, 1, &num_configs);

	//error log 
	if (num_configs == 0)
		return ;

	eglGetConfigAttrib(dpy, config, EGL_WIDTH, (EGLint*)&(windowParams.width));
	eglGetConfigAttrib(dpy, config, EGL_HEIGHT, (EGLint*)&(windowParams.height));

	ctx = eglCreateContext(dpy, config, NULL, NULL);
	surface = eglCreateWindowSurface(dpy, config, 0, NULL);
	eglMakeCurrent(dpy, surface, surface, ctx);
}

void GLRenderWindow::destoryGLResource()
{
	eglTerminate(dpy);
}


