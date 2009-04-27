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
