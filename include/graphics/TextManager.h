#ifndef _TEXTMANAGER_H_
#define _TEXTMANAGER_H_

#include "GraphicsPrerequisites.h"

namespace hare_graphics
{
	class Font;

	class GRAPHICS_API TextManager : public Singleton<TextManager>
	{
		HARE_DECLARE_SINGLETON(TextManager)
	public:
		TextManager();
		~TextManager();

		Font* createFont(const String& name, f32 size);
		void  destroyFont(Font* f);

		void setFontResolution(u32 resolution);
		void setCacheSize(u32 cacheSize);
	private:
		std::list<Font*> fontList;
		u32 fontResolution;
		u32 fontCacheSize;
	};

	GRAPHICS_API TextManager* getTextManager();
}

#endif
