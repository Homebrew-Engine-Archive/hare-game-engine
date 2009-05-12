//***************************************************************
//  File:    SpriteMIMEHandler.h
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
#ifndef _SPRITEMIMEHANDLER_H_
#define _SPRITEMIMEHANDLER_H_ 

class SpriteMIMEHandler : public MIMEHandlerPlugin
{
public:
    SpriteMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

private:

};

#endif