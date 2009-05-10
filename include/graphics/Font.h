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
		//ttfName ttf�ļ����� ttfSize�����С ttfResolution�豸�ֱ��� cacheBufferSize������д�С��������̬����Ĵ�С��
		Font(const String& ttfName, float ttfSize, uint32 ttfResolution, uint32 cacheSize);
		~Font();

		//���ڱ༭ʱ��̬����
		virtual void postLoaded();
        virtual void postEdited(Attribute* attr);

		void setFontFileName(const String& ttfName);

		const String& getFontFileName();

		void setFontSize(float ttfSize);

		float getFontSize();

		void setFontResolution(uint32 ttfResolution);

		uint32 getFontResolution();

		void setCacheSize(uint32 cacheSize);

		//�õ�һ����ģ �� cacheCharQueue��û�����codePoint��ϵͳ�ƶ��滻������õ�
		const CharGlyph& getCharGlyph(uint32 codePoint);

		//�ַ��������
		int getMaxCharWidth();

		//�ַ�����ߴ�
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

		CachedChar willBeFillCachedPos;//���һ����ģʱ�������е�λ�� ��ʼ����λ����(0,0)
		CharGlyph charGlyph;

		bool bInitalize;

	};
}


#endif
