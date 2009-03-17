#include "PCH.h"
#include "TextManager.h"
#include "Font.h"



namespace hare_graphics
{

	HARE_IMPLEMENT_SINGLETON(TextManager)
	
	TextManager::TextManager()
		:fontResolution(100)
		,fontCacheSize(20)
	{

	}

	TextManager::~TextManager()
	{
		std::list<Font*>::iterator it = fontList.begin();
		for (;it != fontList.end(); ++it){
			delete (Font*)(*it);
		}
		fontList.clear();
	}

	Font* TextManager::createFont(const String& name, f32 size)
	{
		Font* font
		try{
			font = new Font(name, size, fontResolution, fontCacheSize);
			fontList.push_back(font);
		}catch(...){
			font = NULL;
		}
		
		return font;
	}

	void TextManager::destroyFont(Font* f)
	{
		std::list<Font*>::iterator it = std::find(fontList.begin(), fontList.end(), f);
		if (it != fontList.end()){
			delete f;
			fontList.erase(it);
		}
	}

	void TextManager::setFontResolution(u32 resolution)
	{
		fontResolution = resolution;
	}

	void TextManager::setCacheSize(u32 cacheSize)
	{
		fontCacheSize = cacheSize;
	}

	TextManager* getTextManager()
	{
		return TextManager::getSingletonPtr();
	}

}