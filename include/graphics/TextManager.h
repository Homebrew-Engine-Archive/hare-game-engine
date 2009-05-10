//***************************************************************
//  File:    TextManager.h
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
#ifndef _TEXTMANAGER_H_
#define _TEXTMANAGER_H_

#include "GraphicsPrerequisites.h"
#include "Font.h"

namespace hare
{
	class GRAPHICS_API TextManager : public Singleton<TextManager>
	{
		HARE_DECLARE_SINGLETON(TextManager)
	public:
		TextManager();
		~TextManager();

		Font* createFont(const String& name, float size);

		void setFontResolution(uint32 resolution);
		void setCacheSize(uint32 cacheSize);
	private:
		uint32 fontResolution;
		uint32 fontCacheSize;
	};

	GRAPHICS_API TextManager* getTextManager();
}

#endif
