//***************************************************************
//  File:    GLSystemManager.h
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
#ifndef _GLSYSTEMMANAGER_H_
#define _GLSYSTEMMANAGER_H_


class GLSystemManager : public SystemManager
{
    HARE_DECLARE_DYNAMIC_CLASS(GLSystemManager)

public:
	GLSystemManager();

	virtual ~GLSystemManager();

	virtual RenderWindow* createRenderWindow(const WindowParams& params);

	virtual void destoryRenderWindow(RenderWindow* renderWindow);

	virtual Texture* createTexture();

	virtual int hareRunFrame();

	virtual void hareRun();
};


#endif
