//***************************************************************
//  File:    RenderWindow.cpp
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
#include "RenderWindow.h"

namespace hare
{
    void WindowParams::setCustomData(const String& key, const String& val)
    {
        customData[key] = val;
    }

    String WindowParams::getCustomData(const String& key)
    {
        ParamList::iterator it = customData.find(key);
        if (it != customData.end())
            return it->second;
        else
            return StringUtil::EMPTY;
    }

    bool WindowParams::hasCustomData(const String& key)
    {
        return customData.find(key) != customData.end();
    }

	RenderWindow::RenderWindow()
		:isMainWnd(false)
        ,isExternal(false)
		,sceneManager(NULL)
		,cameraPos(0, 0)
	{
	}

	RenderWindow::~RenderWindow()
	{
        sceneManager = NULL;
	}

	void RenderWindow::moveCamera(float dx, float dy)
	{
	    cameraPos.x += dx; cameraPos.y += dy;
	}

    void RenderWindow::moveCameraTo(float x, float y)
	{
		cameraPos.x = x; cameraPos.y = y;
	}

    const PointF& RenderWindow::getCameraPos()
    {
        return cameraPos;
    }

}
