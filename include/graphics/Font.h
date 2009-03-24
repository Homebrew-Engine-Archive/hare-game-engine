#ifndef _FONT_H_
#define _FONT_H_

#include "GraphicsPrerequisites.h"

#include "Texture.h"
#include "Shader.h"
#include "DataHolder.h"

namespace hare_graphics
{
	struct FontResource;

	struct CharGlyph
	{
		f32 baselineX;
		f32 bear_left;
		f32 bear_advanceX;

		f32 baselineY;
		f32 bear_top;
		f32 bear_advanceY;

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

			u32 codePoint;

			CharGlyph charGlyph;

			CachedChar()
				:x(0),y(0)
			{

			}
		};
	public:

		Font();
		//ttfName ttf�ļ����� ttfSize�����С ttfResolution�豸�ֱ��� cacheBufferSize������д�С��������̬����Ĵ�С��
		Font(const String& ttfName, f32 ttfSize, u32 ttfResolution, u32 cacheSize);
		~Font();

		//���ڱ༭ʱ��̬����
		virtual void postLoaded();
        virtual void postEdited(Attribute* attr);

		void setFontFileName(const String& ttfName);

		const String& getFontFileName();

		void setFontSize(f32 ttfSize);

		f32 getFontSize();

		void setFontResolution(u32 ttfResolution);

		u32 getFontResolution();

		void setCacheSize(u32 cacheSize);

		//�õ�һ����ģ �� cacheCharQueue��û�����codePoint��ϵͳ�ƶ��滻������õ�
		const CharGlyph& getCharGlyph(u32 codePoint);

		//�ַ��������
		int getMaxCharWidth();

		//�ַ�����ߴ�
		u32 getCacheBufferSize();

		Texture* getFontTexture();

		const ShaderParams& getFontExtParams();

	protected:
		void addCharGlyph(u32 codePoint);

		void advanceFillCache(u32 codePointBegin, u32 codePointEnd);

		void initalzeResource();

		void destoryResource();

	protected:
		String fontFile;
		f32 fontSize;
		u32 cacheBufferSize;
		u32 resolution;
		int maxCharWidth;
		ShaderParams shaderParams;

		std::deque<CachedChar> cacheCharQueue;
		Texture::Ptr texCache;
		u32 numCharPerLine;

		FontResource* fontResource; 
		DataHolder  input;

		CachedChar willBeFillCachedPos;//���һ����ģʱ�������е�λ�� ��ʼ����λ����(0,0)
		CharGlyph charGlyph;

		bool bInitalize;

	};
}


#endif
