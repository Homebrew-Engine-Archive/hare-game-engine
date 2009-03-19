#ifndef _FONT_H_
#define _FONT_H_

#include "GraphicsPrerequisites.h"

#include "Texture.h"
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "DataHolder.h"

namespace hare_graphics
{
	struct CharGlyph
	{
		f32 baselineX;
		f32 bear_left;
		f32 bear_advanceX;

		f32 baselineY;
		f32 bear_top;
		f32 bear_advanceY;

		Rect<f32> recGlyph;
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

			f32 baselineX;
			f32 bear_left;
			f32 bear_advanceX;

			f32 baselineY;
			f32 bear_top;
			f32 bear_advanceY;

			CachedChar()
				:x(0),y(0)
			{

			}

		};
	public:

		Font();
		//ttfName ttf文件名字 ttfSize字体大小 ttfResolution设备分辨率 cacheBufferSize缓冲队列大小（决定动态纹理的大小）
		Font(const String& ttfName, f32 ttfSize, u32 ttfResolution, u32 cacheSize);
		~Font();

		//用于编辑时动态加载
		void postEdit();

		void setFontFileName(const String& ttfName);

		const String& getFontFileName();

		void setFontSize(f32 ttfSize);

		f32 getFontSize();

		void setFontResolution(u32 ttfResolution);

		u32 getFontResolution();

		void setCacheSize(u32 cacheSize);

		//得到一个字模 若 cacheCharQueue中没有这个codePoint则系统制动替换掉最不常用的
		const CharGlyph& getCharGlyph(u32 codePoint);

		//字符的最大宽度
		int getMaxCharWidth();

		//字符缓冲尺寸
		u32 getCacheBufferSize();

		Texture* getFontTexture();

		const ShaderParams& getFontExtParams();

	protected:
		void addCharGlyph(u32 codePoint);

		void advanceFillCache(u32 codePointBegin, u32 codePointEnd);

	protected:
		String fontName;
		f32 fontSize;
		u32 cacheBufferSize;
		u32 resolution;
		int maxCharWidth;
		ShaderParams shaderParams;

		std::deque<CachedChar> cacheCharQueue;
		Texture::Ptr texCache;
		u32 numCharPerLine;

		FT_Face		face;
		FT_Library	ftLibrary;
		DataHolder  input;

		CachedChar willBeFillCachedPos;//添加一个字模时在纹理中的位置 初始化的位置是(0,0)
		CharGlyph charGlyph;

	};
}


#endif
