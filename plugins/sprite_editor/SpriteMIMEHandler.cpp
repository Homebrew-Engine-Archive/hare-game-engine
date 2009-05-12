//***************************************************************
//  File:    SpriteMIMEHandler.cpp
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
#include "SpriteMIMEHandler.h"

SpriteMIMEHandler::SpriteMIMEHandler()
{
    
}


bool SpriteMIMEHandler::canHandle(const wxString& filename) const
{
    return false;
}

bool SpriteMIMEHandler::openFile(const wxString& filename)
{
    return false;
}