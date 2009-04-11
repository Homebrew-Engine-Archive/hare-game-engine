#include "PCH.h"
#include "D3DRenderWindow.h"
#include "D3DRenderSystem.h"
#include "D3DSystemManager.h"


namespace hare_d3d
{
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
		D3DRenderWindow* renderWindow = (D3DRenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
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
	



	D3DRenderWindow::D3DRenderWindow(LPDIRECT3DDEVICE9 pD3DDevice)
		:pRenderSurface(NULL)
		,pDepthStencilSurface(NULL)
		,pSwapChain(NULL)
	{
		isMainWnd = (pD3DDevice == NULL);//���Ϊ����Ϊ������
	}

	D3DRenderWindow::~D3DRenderWindow()
	{
		destoryD3DResource();

		//�Ƴ��Լ����豸�������
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

		D3Dpp.Windowed	= !windowParams.bFullScreen;
		D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		D3Dpp.BackBufferCount = 1;
		D3Dpp.BackBufferFormat = d3ddm.Format;
		D3Dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//��Ҫ���ز���
		D3Dpp.hDeviceWindow = (HWND)windowParams.hwnd;
		D3Dpp.BackBufferWidth = windowParams.width;
		D3Dpp.BackBufferHeight = windowParams.height;
		D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
  		if (windowParams.bZbuffer){
			D3Dpp.EnableAutoDepthStencil = TRUE;	
			D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;
		}



	}

	void D3DRenderWindow::create(const WindowParams& params)
	{
		//assert(params.hwnd);
		HWND hwnd;

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

            u32 style = WS_OVERLAPPEDWINDOW;
            if (params.bFullScreen){
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

		windowParams.hwnd = (u32)hwnd;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (u32)this);

		//������Ϣ�Ժ��
		//�������ļ��еõ� D3Dpp
		initalizeD3DConfigParam();

		createD3DResource();

		//ע���Լ����豸�������
		DeviceManager::getSingletonPtr()->registerDeviceObject(this);
	}

	void D3DRenderWindow::destoryWindow()
	{
        sceneManager = NULL;

		if (getIsMainWnd()){
            if (!isExternal){
			    //��������������˳�����
			    PostQuitMessage(0);
            }
		}else{
			//���������ͷ���Դ
			D3DSystemManager::getSingletonPtr()->destoryRenderWindow(this);	
		}
	}

	void D3DRenderWindow::resize(u32 w, u32 h)
	{
		assert(w > 0 && h > 0);

		LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
		assert(pD3DDevice);

		windowParams.width = w;
		windowParams.height = h;

		D3Dpp.BackBufferWidth = windowParams.width;
		D3Dpp.BackBufferHeight = windowParams.height;

        static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->resetDevice();
		//�Ժ�������ļ��еĵ��Ƿ��ؽ��󱳻�����Ƿ���е�����

	}

	void D3DRenderWindow::swapBuffer()
	{
		LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
		
		if (isMainWnd){
			pD3DDevice->Present(NULL, NULL, 0, NULL);
		}else{
			pSwapChain->Present(NULL, NULL, NULL, NULL, 0);
		}

		//���d3d����
		D3DRenderSystem::getSingletonPtr()->clear(windowParams.bZbuffer);
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

		//û��Z pDepthStencilSurfaceΪ��
		hr = pD3DDevice->SetDepthStencilSurface(pDepthStencilSurface);

		if (FAILED(hr))
			assert(false);

		D3DXMATRIX matTEMP;

		D3DXMatrixScaling(&MatProj, 1.0f, -1.0f, 1.0f);
		D3DXMatrixTranslation(&matTEMP, -0.5f, windowParams.height + 0.5f, 0.0f);
		D3DXMatrixMultiply(&MatProj, &MatProj, &matTEMP);

		D3DXMatrixOrthoOffCenterRH(&matTEMP, 0, (f32)windowParams.width, 0, 
			(f32)windowParams.height, 0.0f, 1.0f);//����ͶӰ

		D3DXMatrixMultiply(&MatProj, &MatProj, &matTEMP);
		D3DXMatrixIdentity(&MatView);


		pD3DDevice->SetTransform(D3DTS_VIEW, &MatView);
		pD3DDevice->SetTransform(D3DTS_PROJECTION, &MatProj);

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
		return (HWND)windowParams.hwnd;
	}

	void D3DRenderWindow::createD3DResource()
	{
		static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->createDevice(this);
	}

	void D3DRenderWindow::destoryD3DResource()
	{
		SAFE_RELEASE(pRenderSurface);
		SAFE_RELEASE(pDepthStencilSurface);

		if (isMainWnd){//������

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

}