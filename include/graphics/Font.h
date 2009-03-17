#ifndef _FONT_H_
#define _FONT_H_

#include "GraphicsPrerequisites.h"

#include "Texture.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "DataHolder.h"

namespace hare_graphics
{
	struct CharGlyph
	{
		Rect<f32> recGlyph;
		Texture::Ptr texGlyph;
	};

	class GRAPHICS_API Font 
	{
	protected:
		struct CachedChar
		{
			int x;
			int y;
			Rect<f32> recGlyph;
			u32 codePoint;

			CachedChar()
				:x(0),y(0)
			{

			}

		};
	public:
		//ttfName ttf�ļ����� ttfSize�����С ttfResolution�豸�ֱ��� cacheBufferSize������д�С��������̬����Ĵ�С��
		Font(const String& ttfName, f32 ttfSize, u32 ttfResolution, u32 cacheSize);
		~Font();

		//�õ�һ����ģ �� cacheCharQueue��û�����codePoint��ϵͳ�ƶ��滻������õ�
		const CharGlyph& getCharGlyph(u32 codePoint);

		//��������
		const String& getFontName();

		//����ߴ�
		f32 getFonSize();

		//�ַ��������
		int getMaxCharWidth();

		//�ַ�����ߴ�
		u32 getCacheBufferSize();

	protected:
		void addCharGlyph(u32 codePoint);

		void advanceFillCache(u32 codePointBegin, u32 codePointEnd);

	protected:
		String fontName;
		f32 fontSize;
		int maxCharWidth;
		u32 cacheBufferSize;
		std::deque<CachedChar> cacheCharQueue;
		Texture::Ptr texCache;
		u32 numCharPerLine;

		FT_Face		face;
		FT_Library	ftLibrary;
		DataHolder  input;

		CachedChar willBeFillCachedPos;//���һ����ģʱ�������е�λ�� ��ʼ����λ����(0,0)
		CharGlyph charGlyph;

		int baseline;//�ַ��Ļ���
	};
}


#endif
