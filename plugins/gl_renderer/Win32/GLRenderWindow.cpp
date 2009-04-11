#include "PCH.h"
#include "GLRenderWindow.h"

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
        sizeof(PIXELFORMATDESCRIPTOR),  // ������ʽ�������Ĵ�С
        1,                              // �汾��
        PFD_DRAW_TO_WINDOW |            // ��ʽ֧�ִ���
        PFD_SUPPORT_OPENGL |            // ��ʽ����֧��OpenGL
        PFD_DOUBLEBUFFER,               // ����֧��˫����
        PFD_TYPE_RGBA,                  // ���� RGBA ��ʽ
        COLOR_DEPTH,                    // ѡ��ɫ�����
        0, 0, 0, 0, 0, 0,               // ���Ե�ɫ��λ
        0,                              // ��Alpha����
        0,                              // ����Shift Bit
        0,                              // ���ۼӻ���
        0, 0, 0, 0,                     // ���Ծۼ�λ
        windowParams.bZbuffer ? 16 : 0, // 16λ Z-���� (��Ȼ���)
        0,                              // ���ɰ建��
        0,                              // �޸�������
        PFD_MAIN_PLANE,                 // ����ͼ��
        0,                              // Reserved
        0, 0, 0                         // ���Բ�����
    };

    pfd = tmpPFD;
}

void GLRenderWindow::create(const WindowParams& params)
{
    HWND hwnd;
    bool bFullScreen = params.bFullScreen;

    if (params.hwnd == NULL){//������ھ��Ϊ�����ƶ�����

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
        hwnd = (HWND)params.hwnd;
        isExternal = true;
    }

    windowParams = params;

    windowParams.hwnd = (uint32)hwnd;

    windowParams.bFullScreen = bFullScreen;

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (uint32)this);

    //get information from config
    initalizeGLConfigParam();

    createGLResource();
}

void GLRenderWindow::setProjection()
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity (); 
    
    //NOTE!! glOrtho funcation last tow args is used as negative
    if (windowParams.width <= windowParams.height)                                                                    //near far
        glOrtho(-50.0, 50.0, -50.0*(GLfloat)windowParams.height/(GLfloat)windowParams.width, 50.0*(GLfloat)windowParams.height/(GLfloat)windowParams.width,-1.0,1.0); 
    else
        glOrtho(-50.0*(GLfloat)windowParams.width/(GLfloat)windowParams.height, 50.0*(GLfloat)windowParams.width/(GLfloat)windowParams.height, -50.0, 50.0,-1.0,1.0); 

    glMatrixMode (GL_MODELVIEW);
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

    GLbitfield flag = GL_COLOR_BUFFER_BIT;
    if (windowParams.bZbuffer){
        flag |= GL_DEPTH_BUFFER_BIT;
    }
    glClear(flag);
}

void GLRenderWindow::destoryWindow()
{
    sceneManager = NULL;

    if (getIsMainWnd()){
        if (!isExternal){
            //exit main Process
            PostQuitMessage(0);
        }
    }else{
        //destory window 
        //GLSystemManager::getSingletonPtr()->destoryRenderWindow(this);	
    }
    //NOTE!! can't modify member value affter this funcation
}

void GLRenderWindow::active()
{
    if(!wglMakeCurrent(hDC,hRC)){
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't active RenderContext!", "GLRenderWindow::active");
    }

    setProjection();
}

void GLRenderWindow::inactive()
{
    
}

HWND GLRenderWindow::getWindowHandle()
{
    return (HWND)windowParams.hwnd;
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
    GLuint PixelFormat;

    if (!(hDC=GetDC((HWND)windowParams.hwnd))){	// get DeviceContext
        MessageBox(NULL, "can't get DeviceContext", "error", MB_OK|MB_ICONEXCLAMATION);
        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't get DeviceContext!", "GLRenderWindow::createGLResource");
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd))){ // Windows search PixelFormat 
        MessageBox(NULL, "can't search PixelFormat", "error",MB_OK|MB_ICONEXCLAMATION);
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

}

void GLRenderWindow::destoryGLResource()
{

    if (windowParams.bFullScreen){
        ChangeDisplaySettings(NULL, 0);	
    }

    if (hRC){
        if (!wglMakeCurrent(NULL, NULL)){
            HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't make current OpenGL render Context NULL!", "GLRenderWindow::destoryGLResource"); 
        }

        if (!wglDeleteContext(hRC)){
            HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "can't destory OpenGL render Context!", "GLRenderWindow::destoryGLResource"); 
        }
        hRC = NULL;							
    }

    if (hDC && !ReleaseDC((HWND)windowParams.hwnd, hDC)){
        hDC = NULL;
    }

    if (windowParams.hwnd && !DestroyWindow((HWND)windowParams.hwnd)){
        windowParams.hwnd = NULL;							
    }

}




