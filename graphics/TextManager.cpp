#include "PCH.h"
#include "TextManager.h"
#include "Font.h"



namespace hare
{

	HARE_IMPLEMENT_SINGLETON(TextManager)

	TextManager::TextManager()
		:fontResolution(100)
		,fontCacheSize(10)
	{

	}

	TextManager::~TextManager()
	{

	}

	Font* TextManager::createFont(const String& name, float size)
	{
		Font* font;
		try{
			font = new Font(name, size, fontResolution, fontCacheSize);
		}catch(...){
			font = NULL;
		}

		return font;
	}

	void TextManager::setFontResolution(uint32 resolution)
	{
		fontResolution = resolution;
	}

	void TextManager::setCacheSize(uint32 cacheSize)
	{
		fontCacheSize = cacheSize;
	}

	TextManager* getTextManager()
	{
		return TextManager::getSingletonPtr();
	}
}
