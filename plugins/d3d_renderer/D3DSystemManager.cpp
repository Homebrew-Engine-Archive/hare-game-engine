//***************************************************************
//  File:    D3DSystemManager.cpp
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
#include "D3DSystemManager.h"
#include "D3DRenderSystem.h"
#include "D3DRenderWindow.h"
#include "D3DTexture.h"

HARE_IMPLEMENT_DYNAMIC_CLASS(D3DSystemManager, SystemManager, 0)
{
}

D3DSystemManager::D3DSystemManager()
{
	renderSystem = new D3DRenderSystem;
	renderSystem->initalize();
}

D3DSystemManager::~D3DSystemManager()
{
	delete pPrimaryWindow;
	pPrimaryWindow = NULL;
	SecondaryWindowList::iterator it = secondaryWindowList.begin();
	for (;it != secondaryWindowList.end(); ++it){
		delete *it;
	}
	secondaryWindowList.clear();

	renderSystem->release();
	delete renderSystem;
}

RenderWindow* D3DSystemManager::createRenderWindow(const WindowParams& params)
{
	D3DRenderWindow* window = new D3DRenderWindow(static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice());

	window->create(params);

	if (window->getIsMainWnd()){
		pPrimaryWindow = window;
	}else{
		secondaryWindowList.push_back(window);
	}

	return window;
}

void D3DSystemManager::destoryRenderWindow(RenderWindow* renderWindow)
{
	//不能在这销毁主窗口
	//assert(renderWindow != pPrimaryWindow && pPrimaryWindow != NULL);

	SecondaryWindowList::iterator it = secondaryWindowList.begin();
	for (;it != secondaryWindowList.end(); ++it){
		if (renderWindow == (*it)){
			delete *it;
			secondaryWindowList.erase(it);
			break;
		}
	}
}

Texture* D3DSystemManager::createTexture()
{
	return RenderSystem::getSingletonPtr()->createTexture();
}

int D3DSystemManager::hareRunFrame()
{
    D3DRenderSystem* d3drs = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr());
    if (!d3drs)
        return 1;
    
    LPDIRECT3DDEVICE9 d3ddev = d3drs->getD3DDevice();
    if (!d3ddev)
        return 1;
	
    HRESULT hr = d3ddev->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST){
		return 1;
	}else if (hr == D3DERR_DEVICENOTRESET){
		d3drs->resetDevice();
	}

	return SystemManager::hareRunFrame();
}

void D3DSystemManager::hareRun()
{
	MSG Message;

	for (;;){	
		if (PeekMessage(&Message, 0, 0, 0, PM_NOREMOVE)){
			if (!GetMessage(&Message, NULL, 0, 0)){
				break;
			}
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		} else {
			int ret = hareRunFrame();
				
			if (ret == 0)
				break;
		}
	}
}


