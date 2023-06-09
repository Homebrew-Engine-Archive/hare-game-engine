//***************************************************************
//  File:    D3DRenderWindow.cpp
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "D3DRenderWindow.h"
#include "D3DRenderSystem.h"
#include "D3DSystemManager.h"



static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	D3DRenderWindow* renderWindow = (D3DRenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch( uMsg )
	{
	case WM_SIZE:
		if (renderWindow && !renderWindow->getWindowParams().fullScreen){// not Alt + Tab
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




D3DRenderWindow::D3DRenderWindow(LPDIRECT3DDEVICE9 pD3DDevice)
	: pRenderSurface(NULL), pDepthStencilSurface(NULL), pSwapChain(NULL), hWindow(NULL)
{
	isMainWnd = (pD3DDevice == NULL);//如果为空则为主窗口
}

D3DRenderWindow::~D3DRenderWindow()
{
	destoryD3DResource();

	//移除自己到设备对象管理
	DeviceManager::getSingletonPtr()->unregisterDeviceObject(this);
}

void D3DRenderWindow::initalizeD3DConfigParam()
{
	LPDIRECT3D9 pD3DInterface = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DInterface();

	assert(pD3DInterface);

	ZeroMemory(&D3Dpp, sizeof(D3DPRESENT_PARAMETERS));
	
	D3DDISPLAYMODE d3ddm;

	if (FAILED(pD3DInterface->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT, &d3ddm))){
		assert(false);
	}

	D3Dpp.Windowed	= !windowParams.fullScreen;
	D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3Dpp.BackBufferCount = 1;
	D3Dpp.BackBufferFormat = d3ddm.Format;
	D3Dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//不要多重采样
	D3Dpp.hDeviceWindow = hWindow;
	D3Dpp.BackBufferWidth = windowParams.width;
	D3Dpp.BackBufferHeight = windowParams.height;
	D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //D3DPRESENT_INTERVAL_IMMEDIATE;
	
  	if (windowParams.hasZbuffer){
		D3Dpp.EnableAutoDepthStencil = TRUE;	
		D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}



}

void D3DRenderWindow::create(const WindowParams& params)
{
    windowParams = params;

	if (!windowParams.hasCustomData("WINDOW")) {//如果窗口句柄为空则制动创建

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

        uint32 style = WS_OVERLAPPEDWINDOW;
        if (windowParams.fullScreen) {
            style = WS_POPUP;
        }

		hWindow = CreateWindow("HareRenderWindow", windowParams.title.c_str(), 
			style, CW_USEDEFAULT, 0, windowParams.width, windowParams.height, NULL, NULL, hInstance, NULL);

		RECT  rect;


		int xL = (GetSystemMetrics(SM_CXSCREEN) - windowParams.width) / 2;
		int yT = (GetSystemMetrics(SM_CYSCREEN) - windowParams.height) / 2;
		int xR = xL + windowParams.width;
		int yB = yT + windowParams.height;

		SetRect(&rect, xL, yT, xR, yB);

		AdjustWindowRectEx(&rect, GetWindowLong(hWindow, GWL_STYLE), 
			(GetMenu(hWindow) != NULL), GetWindowLong(hWindow, GWL_EXSTYLE));

		MoveWindow(hWindow, rect.left, rect.top, rect.right - rect.left,
			rect.bottom - rect.top, TRUE);

		ShowWindow(hWindow, SW_NORMAL);

		UpdateWindow(hWindow);

        isExternal = false;
	} else {
		hWindow = (HWND)strtoul(windowParams.getCustomData("WINDOW").c_str(), 0, 10);
        isExternal = true;
	}

	SetWindowLongPtr(hWindow, GWLP_USERDATA, (uint32)this);

	//配置信息以后加
	//从配置文件中得到 D3Dpp
	initalizeD3DConfigParam();

	createD3DResource();

	//注册自己到设备对象管理
	DeviceManager::getSingletonPtr()->registerDeviceObject(this);
}

void D3DRenderWindow::destoryWindow()
{
    sceneManager = NULL;

	if (getIsMainWnd()){
        //if (!isExternal){
			//如果是主窗口则退出程序
			//PostQuitMessage(0);
			getHareApp()->end();
        //}
	}else{
		//副窗口则释放资源
		D3DSystemManager::getSingletonPtr()->destoryRenderWindow(this);	
	}
}

bool D3DRenderWindow::getCustomData(const String& key, void* data)
{
    if (key == "WINDOW")
    {
        *static_cast<HWND*>(data) = hWindow;
        return true;
    }

    return false;
}

void D3DRenderWindow::resize(uint32 w, uint32 h)
{
	assert(w > 0 && h > 0);

	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
	assert(pD3DDevice);

	windowParams.width = w;
	windowParams.height = h;

	D3Dpp.BackBufferWidth = windowParams.width;
	D3Dpp.BackBufferHeight = windowParams.height;

    static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->resetDevice();
	//以后从配置文件中的到是否重建后背缓冲和是否剧中的问题
}

void D3DRenderWindow::swapBuffer()
{
	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
	
	if (isMainWnd){
		pD3DDevice->Present(NULL, NULL, 0, NULL);
	}else{
		pSwapChain->Present(NULL, NULL, NULL, NULL, 0);
	}
    //清空d3d场景
    D3DRenderSystem::getSingletonPtr()->clear(windowParams.hasZbuffer);
}

void D3DRenderWindow::active()
{
	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
	assert(pD3DDevice);

	D3DRenderSystem::getSingletonPtr()->beginFrame();

	HRESULT hr;
	
	hr = pD3DDevice->SetRenderTarget(0, pRenderSurface); 

	if (FAILED(hr))
		assert(false);

	//没有Z pDepthStencilSurface为空
	hr = pD3DDevice->SetDepthStencilSurface(pDepthStencilSurface);

	if (FAILED(hr))
		assert(false);

    int l = (int)cameraPos.x;
    int r = windowParams.width + (int)cameraPos.x;
    int b = -(int)cameraPos.y;
    int t = windowParams.height - (int)cameraPos.y;

    RenderSystem::getSingletonPtr()->setProjection((float)l, (float)r, (float)b, (float)t);

	D3DRenderSystem::getSingletonPtr()->setCurRenderWindow(this);

}

void D3DRenderWindow::inactive()
{
	D3DRenderSystem::getSingletonPtr()->render();

	D3DRenderSystem::getSingletonPtr()->endFrame();
}

void D3DRenderWindow::beforeResetDevice()
{
	destoryD3DResource();
}

void D3DRenderWindow::afterResetDevice()
{
	createD3DResource();
}

HWND D3DRenderWindow::getWindowHandle()
{
	return hWindow;
}

void D3DRenderWindow::createD3DResource()
{
	static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->createDevice(this);
}

void D3DRenderWindow::destoryD3DResource()
{
	SAFE_RELEASE(pRenderSurface);
	SAFE_RELEASE(pDepthStencilSurface);

	if (isMainWnd){//主窗口

	}else{
		SAFE_RELEASE(pSwapChain);
	}
}

LPDIRECT3DSURFACE9 D3DRenderWindow::getRenderSurface()
{
	return pRenderSurface;
}

D3DPRESENT_PARAMETERS* D3DRenderWindow::getPresentationParameters()
{
	return &D3Dpp;
}

