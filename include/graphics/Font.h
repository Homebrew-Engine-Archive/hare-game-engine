//***************************************************************
//  File:    Font.h
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
#ifndef _FONT_H_
#define _FONT_H_

#include "GraphicsPrerequisites.h"

#include "Texture.h"
#include "Shader.h"
#include "DataHolder.h"

namespace hare
{
	struct FontResource;

	struct CharGlyph
	{
		float baselineX;
		float bear_left;
		float bear_advanceX;

		float baselineY;
		float bear_top;
		float bear_advanceY;

		RectF recGlyph;
	};

    class GRAPHICS_API Font : public Object
	{
        HARE_DECLARE_DYNAMIC_CLASS(Font)

	protected:
		struct CachedChar
		{
			int x;
			int y;

			uint32 codePoint;

			CharGlyph charGlyph;

			CachedChar()
				:x(0),y(0)
			{

			}
		};
	public:

		Font();
		//ttfName ttf文件名字 ttfSize字体大小 ttfResolution设备分辨率 cacheBufferSize缓冲队列大小（决定动态纹理的大小）
		Font(const String& ttfName, float ttfSize, uint32 ttfResolution, uint32 cacheSize);
		~Font();

		//用于编辑时动态加载
		virtual void postLoaded();
        virtual void postEdited(Attribute* attr);

		void setFontFileName(const String& ttfName);

		const String& getFontFileName();

		void setFontSize(float ttfSize);

		float getFontSize();

		void setFontResolution(uint32 ttfResolution);

		uint32 getFontResolution();

		void setCacheSize(uint32 cacheSize);

		//得到一个字模 若 cacheCharQueue中没有这个codePoint则系统制动替换掉最不常用的
		const CharGlyph& getCharGlyph(uint32 codePoint);

		//字符的最大宽度
		int getMaxCharWidth();

		//字符缓冲尺寸
		uint32 getCacheBufferSize();

		Texture* getFontTexture();

		const ShaderParams& getFontExtParams();

	protected:
		void addCharGlyph(uint32 codePoint);

		void advanceFillCache(uint32 codePointBegin, uint32 codePointEnd);

		void initalzeResource();

		void destoryResource();

	protected:
		String fontFile;
		float fontSize;
		uint32 cacheBufferSize;
		uint32 resolution;
		int maxCharWidth;
		ShaderParams shaderParams;

		std::deque<CachedChar> cacheCharQueue;
		Texture::Ptr texCache;
		uint32 numCharPerLine;

		FontResource* fontResource;
		DataHolder  input;

		CachedChar willBeFillCachedPos;//添加一个字模时在纹理中的位置 初始化的位置是(0,0)
		CharGlyph charGlyph;

		bool bInitalize;

	};
}


#endif
