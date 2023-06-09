//***************************************************************
//  File:    GLRenderWindow.h
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
#ifndef _GLRENDERWINDOW_H_
#define _GLRENDERWINDOW_H_

class GLRenderWindow : public RenderWindow
{
    friend class GLRenderSystem;
public:
    GLRenderWindow(bool isMainWindow);

    virtual ~GLRenderWindow();

    virtual void create(const WindowParams& params);

    virtual void resize(uint32 w, uint32 h);

    virtual void swapBuffer();

    virtual void destoryWindow();

    virtual void active();

    virtual void inactive();

    virtual bool getCustomData(const String& key, void* data);

    HWND getWindowHandle();

    HGLRC getRenderContext();

    PIXELFORMATDESCRIPTOR* getPixelFormatDescriptor();


private:
    void initalizeGLConfigParam();
    void createGLResource();
    void destoryGLResource();

    void setProjection();
private:
    HGLRC hRC;
    HDC   hDC;
    PIXELFORMATDESCRIPTOR pfd;
    HWND hWindow;

};


#endif
