#ifndef _D3DRENDERWINDOW_H_
#define _D3DRENDERWINDOW_H_

#include "DeviceManager.h"


class D3DRenderWindow : public RenderWindow , public DeviceObject
{
	friend class D3DRenderSystem;
public:
	D3DRenderWindow(LPDIRECT3DDEVICE9 pD3DDevice);//pD3DDevice用于标识是否为主窗口

	virtual ~D3DRenderWindow();

	virtual void create(const WindowParams& params);

	virtual void resize(uint32 w, uint32 h);

	virtual void swapBuffer();

	virtual void destoryWindow();

	virtual void active();

	virtual void inactive();

	virtual void beforeResetDevice();

	virtual void afterResetDevice();

	HWND getWindowHandle();

	LPDIRECT3DSURFACE9 getRenderSurface();

	D3DPRESENT_PARAMETERS* getPresentationParameters();

private:
	void initalizeD3DConfigParam();
	void createD3DResource();
	void destoryD3DResource();

private:
	LPDIRECT3DSWAPCHAIN9	pSwapChain;
	LPDIRECT3DSURFACE9		pRenderSurface;
	LPDIRECT3DSURFACE9	    pDepthStencilSurface;
	D3DPRESENT_PARAMETERS	D3Dpp;

	D3DXMATRIX MatView;

	D3DXMATRIX MatProj;
};

#endif

