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
		//ttfName ttf文件名字 ttfSize字体大小 ttfResolution设备分辨率 cacheBufferSize缓冲队列大小（决定动态纹理的大小）
		Font(const String& ttfName, f32 ttfSize, u32 ttfResolution, u32 cacheSize);
		~Font();

		//得到一个字模 若 cacheCharQueue中没有这个codePoint则系统制动替换掉最不常用的
		const CharGlyph& getCharGlyph(u32 codePoint);

		//字体名字
		const String& getFontName();

		//字体尺寸
		f32 getFonSize();

		//字符的最大宽度
		int getMaxCharWidth();

		//字符缓冲尺寸
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

		CachedChar willBeFillCachedPos;//添加一个字模时在纹理中的位置 初始化的位置是(0,0)
		CharGlyph charGlyph;

		int baseline;//字符的基线
	};
}


#endif
