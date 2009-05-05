#include "PCH.h"
#include "GLRenderWindow.h"
#include "../GLRenderSystem.h"
#include "../GLSystemManager.h"

#define COLOR_DEPTH 16

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GLRenderWindow* renderWindow = (GLRenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch( uMsg )
    {
    case WM_SIZE:
        {
            int w = LOWORD(lParam) > 0 ? LOWORD(lParam) : 1;
            int h = HIWORD(lParam) > 0 ? HIWORD(lParam) : 1;

            if (renderWindow)
                renderWindow->resize(w, h);
        }
        break;
    case WM_CLOSE:
        if (renderWindow)
            renderWindow->destoryWindow();
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GLRenderWindow::GLRenderWindow(bool bMainWindow)
    :hRC(NULL)
    ,hDC(NULL)
{
    isMainWnd = bMainWindow;
}

GLRenderWindow::~GLRenderWindow()
{
    destoryGLResource();
}

void GLRenderWindow::initalizeGLConfigParam()
{
    PIXELFORMATDESCRIPTOR tmpPFD = {
        sizeof(PIXELFORMATDESCRIPTOR),  // 上述格式描述符的大小
        1,                              // 版本号
        PFD_DRAW_TO_WINDOW |            // 格式支持窗口
        PFD_SUPPORT_OPENGL |            // 格式必须支持OpenGL
        PFD_DOUBLEBUFFER,               // 必须支持双缓冲
        PFD_TYPE_RGBA,                  // 申请 RGBA 格式
        COLOR_DEPTH,                    // 选定色彩深度
        0, 0, 0, 0, 0, 0,               // 忽略的色彩位
        0,                              // 无Alpha缓存
        0,                              // 忽略Shift Bit
        0,                              // 无累加缓存
        0, 0, 0, 0,                     // 忽略聚集位
        windowParams.bZbuffer ? 16 : 0, // 16位 Z-缓存 (深度缓存)
        0,                              // 无蒙板缓存
        0,                              // 无辅助缓存
        PFD_MAIN_PLANE,                 // 主绘图层
        0,                              // Reserved
        0, 0, 0                         // 忽略层遮罩
    };

    pfd = tmpPFD;
}

void GLRenderWindow::create(const WindowParams& params)
{
    HWND hwnd;
    bool bFullScreen = params.bFullScreen;

    if (params.hwnd == NULL){//如果窗口句柄为空则制动创建

        HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

        WNDCLASS wc;

        wc.style = CS_DBLCLKS;
        wc.lpfnWndProc = WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(0, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = "HareRenderWindow";

        RegisterClass(&wc);

        if (bFullScreen){
            DEVMODE dmScreenSettings;
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
            dmScreenSettings.dmSize         = sizeof(dmScreenSettings);
            dmScreenSettings.dmPelsWidth	= params.width;
            dmScreenSettings.dmPelsHeight	= params.height;
            dmScreenSettings.dmBitsPerPel	= COLOR_DEPTH;  // color depth per pixel
            dmScreenSettings.dmFields       = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

            if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL){
                bFullScreen = false;
            }
        }

        uint32 style = WS_OVERLAPPEDWINDOW;
        if (bFullScreen){
            style = WS_POPUP;
        }

        hwnd = CreateWindow("HareRenderWindow", params.title.c_str(), 
            style, CW_USEDEFAULT, 0, params.width, params.height, NULL, NULL, hInstance, NULL);

        RECT  rect;

        int xL = (GetSystemMetrics(SM_CXSCREEN) - params.width ) / 2;
        int yT = (GetSystemMetrics(SM_CYSCREEN) - params.height) / 2;
        int xR = xL + params.width;
        int yB = yT + params.height;

        SetRect(&rect, xL, yT, xR, yB);

        AdjustWindowRectEx(&rect, GetWindowLong(hwnd, GWL_STYLE), 
            (GetMenu(hwnd) != NULL), GetWindowLong(hwnd, GWL_EXSTYLE));

        MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left,
            rect.bottom - rect.top, TRUE);

        ShowWindow(hwnd, SW_NORMAL);

        UpdateWindow(hwnd);

        isExternal = false;
    } else {
        hwnd = params.hwnd;
        isExternal = true;
    }

    windowParams = params;

    windowParams.hwnd = hwnd;

    windowParams.bFullScreen = bFullScreen;

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (uint32)this);

    //get information from config
    initalizeGLConfigParam();

    createGLResource();
}

void GLRenderWindow::setProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //NB: glOrtho funcation last tow args is used as negative
    //left right bottom top                                                 near far
    glOrtho(-cameraPos.x, (GLfloat)windowParams.width - cameraPos.x, (GLfloat)windowParams.height - cameraPos.y, -cameraPos.y, -1.0, 1.0); 

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
    SwapBuffers(hDC);

    GLRenderSystem::getSingletonPtr()->clear(windowParams.bZbuffer);

}

void GLRenderWindow::destoryWindow()
{
    sceneManager = NULL;

    if (getIsMainWnd()){
        //if (!isExternal){
            //exit main Process
            //PostQuitMessage(0);
			getHareApp()->end();
        //}
    }else{
        //destory window 
        GLSystemManager::getSingletonPtr()->destoryRenderWindow(this);	
    }
    //NB: can't modify member value affter this funcation
}

void GLRenderWindow::active()
{
    if (!wglMakeCurrent(NULL, NULL)){
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't make current OpenGL render Context NULL!", "GLRenderWindow::destoryGLResource"); 
    }

    if(!wglMakeCurrent(hDC,hRC)){
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't active RenderContext!", "GLRenderWindow::active");
    }

    setProjection();

	GLRenderSystem::getSingletonPtr()->setCurRenderWindow(this);
}

void GLRenderWindow::inactive()
{
    GLRenderSystem::getSingletonPtr()->render();
}

HWND GLRenderWindow::getWindowHandle()
{
    return windowParams.hwnd;
}

HGLRC GLRenderWindow::getRenderContext()
{
    return hRC;
}

PIXELFORMATDESCRIPTOR* GLRenderWindow::getPixelFormatDescriptor()
{
    return &pfd;
}

void GLRenderWindow::createGLResource()
{
	HDC old_hdc = wglGetCurrentDC();
	HGLRC old_hrc = wglGetCurrentContext();

    GLuint PixelFormat;

    if (!(hDC=GetDC(windowParams.hwnd))){	// get DeviceContext
        MessageBox(NULL, "can't get DeviceContext", "error", MB_OK|MB_ICONEXCLAMATION);
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't get DeviceContext!", "GLRenderWindow::createGLResource");
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd))){ // Windows search PixelFormat 
        MessageBox(NULL, "can't search PixelFormat", "error", MB_OK|MB_ICONEXCLAMATION);
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't search PixelFormat!", "GLRenderWindow::createGLResource");      
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd)){ // set PixelFormat
        MessageBox(NULL, "can't set PixelFormat", "error", MB_OK|MB_ICONEXCLAMATION);
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't set PixelFormat!", "GLRenderWindow::createGLResource"); 
    }

    if (!(hRC=wglCreateContext(hDC))){ // create Render Context
        MessageBox(NULL, "can't create OpenGL render Context", "error", MB_OK|MB_ICONEXCLAMATION);
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't create OpenGL render Context!", "GLRenderWindow::createGLResource"); 
    }

	if (!wglMakeCurrent(hDC, hRC))
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't wglMakeCurrent context!", "GLRenderWindow::createGLResource"); 

	if (old_hrc && old_hrc != hRC)
	{
		// Restore old context
		if (!wglMakeCurrent(old_hdc, old_hrc))
            HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't Restore old context!", "GLRenderWindow::createGLResource"); 
		// Share lists with old context
		if (!wglShareLists(old_hrc, hRC))
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't Share lists with old context!", "GLRenderWindow::createGLResource"); 
		
		if (!wglMakeCurrent(hDC, hRC))
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't wglMakeCurrent context!", "GLRenderWindow::createGLResource"); 
	}

	if (isMainWnd){
        glewInit();
	}

	(static_cast<GLRenderSystem*>(RenderSystem::getSingletonPtr()))->initalizeParam(windowParams.bZbuffer);

    wglSwapIntervalEXT(0);  

}

void GLRenderWindow::destoryGLResource()
{

    if (windowParams.bFullScreen){
        ChangeDisplaySettings(NULL, 0);	
    }

    if (hRC){
        //if (!wglMakeCurrent(NULL, NULL)){
        //    HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't make current OpenGL render Context NULL!", "GLRenderWindow::destoryGLResource"); 
        //}

        if (!wglDeleteContext(hRC)){
            HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't destory OpenGL render Context!", "GLRenderWindow::destoryGLResource"); 
        }
        hRC = NULL;							
    }

    if (hDC && !ReleaseDC(windowParams.hwnd, hDC)){
        hDC = NULL;
    }

	if (!isExternal && windowParams.hwnd && !DestroyWindow(windowParams.hwnd)){
		windowParams.hwnd = NULL;							
	}	

}




