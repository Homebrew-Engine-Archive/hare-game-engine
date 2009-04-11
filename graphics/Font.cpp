#include "PCH.h"
#include "Font.h"
#include "SystemManager.h"
#include <ft2build.h>
#include FT_FREETYPE_H


#define PER_WIDTH 64
#define CODEPOINT_QUAD L'A' + 63

namespace hare_graphics
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(Font, Object, 0)
    {
        HARE_META_F(fontFile, String, propFSUrl)
        HARE_META(fontSize, float)
        HARE_META(cacheBufferSize, uint32)
        HARE_META(resolution, uint32)
    }

	struct FontResource
	{
		FT_Face		face;
		FT_Library	ftLibrary;
	};

	Font::Font()
		:bInitalize(false)
	{
		fontResource = new FontResource;
	}

	Font::Font(const String& ttfName, float ttfSize, uint32 ttfResolution, uint32 cacheSize)
		:fontFile(ttfName)
		,fontSize(ttfSize)
		,cacheBufferSize(cacheSize)
        ,resolution(ttfResolution)
        ,bInitalize(false)
	{
		fontResource = new FontResource;

		postLoaded();
	}

	Font::~Font()
	{
		destoryResource();

		delete fontResource;
	}

	void Font::initalzeResource()
	{
		shaderParams.AlphaBlendEnable = true;
		shaderParams.AlphaTestEnable  = false;

		String tmpExt;
		String fontFileBase;
		StringUtil::splitFilename(fontFile, fontFileBase, tmpExt);
		StringUtil::toLowerCase(tmpExt);
		assert(tmpExt == "ttf");

		FT_Error ftResult = FT_Init_FreeType(&(fontResource->ftLibrary));
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not init FreeType library!", "Font::Font FT_Init_FreeType");

		bInitalize = true;
		/*文件系统打开文件将数据读进input
		*/
		FileSystem* fs = getFileSystem();
		FileHandle fh = fs->openFile(fontFile, FM_Read);

		if (!fh)
			HARE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "Could not open FontFile!", "Font::Font fs->openFile");

		int size = fs->size(fh);
		input.allocate(size);
		fs->readFile(fh, (char*)input.getData(), size, 1);
		fs->closeFile(fh);

		ftResult = FT_New_Memory_Face(fontResource->ftLibrary, (FT_Byte*)input.getData(), (FT_Long)input.getSize(), 0, &(fontResource->face));
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not open font face!", "Font::Font FT_New_Memory_Face");

		FT_F26Dot6  ftSize = (FT_F26Dot6)(fontSize * (1 << 6));
		ftResult = FT_Set_Char_Size( fontResource->face, ftSize, 0, resolution, resolution );
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not set char size!", "Font::Font FT_Set_Char_Size");

		int max_height = 0, max_width = 0;

		//随即抽取几个中文字符选出最大的宽度和空隙
		wchar_t ch = L'赢';
		for (wchar_t count = ch - 50; count < ch + 50; ++count){
			ftResult = FT_Load_Char( fontResource->face, count, FT_LOAD_RENDER);
			if (ftResult)
				continue;

			if (fontResource->face->glyph->bitmap.rows > max_height)
				max_height = fontResource->face->glyph->bitmap.rows;

			if (fontResource->face->glyph->bitmap.width > max_width)
				max_width = fontResource->face->glyph->bitmap.width;		
		}

		ch = L'A';
		for (wchar_t count = ch; count < ch + 63; ++count){
			ftResult = FT_Load_Char(fontResource->face, count, FT_LOAD_RENDER);
			if (ftResult)
				continue;

			if (fontResource->face->glyph->bitmap.rows > max_height)
				max_height = fontResource->face->glyph->bitmap.rows;

			if (fontResource->face->glyph->bitmap.width > max_width)
				max_width = fontResource->face->glyph->bitmap.width;		
		}

		//最大字符宽度
		maxCharWidth = max(max_width, max_height);

		//纹理尺寸
		uint32 texSize = MathUtil::firstPO2From(
            (uint32)MathUtil::sqrtf((float)cacheBufferSize) * maxCharWidth);

		//纹理每行装的字符数
		numCharPerLine = texSize / maxCharWidth;

		//字符实际缓冲大小
		cacheBufferSize = numCharPerLine * numCharPerLine; 

		texCache = SystemManager::getSingletonPtr()->createTexture();

		texCache->create(texSize, texSize, HPF_A8R8G8B8);

		ch = L'A';
		advanceFillCache(ch, ch + 63);
	}

	void Font::destoryResource()
	{
		if (bInitalize){
			FT_Done_FreeType(fontResource->ftLibrary);
			bInitalize = false;
            cacheCharQueue.clear();
			willBeFillCachedPos.x = 0;
			willBeFillCachedPos.y = 0;
			texCache = NULL;
		}
	}

	void Font::postLoaded()
	{
		destoryResource();

		initalzeResource();
	}

    void Font::postEdited(Attribute* attr)
    {
        postLoaded();
    }

	void Font::setFontFileName(const String& ttfName)
	{
		fontFile = ttfName;
	}

	const String& Font::getFontFileName()
	{
		return fontFile;
	}

	void Font::setFontSize(float ttfSize)
	{
		fontSize = ttfSize;
	}

	float Font::getFontSize()
	{
		return fontSize;
	}

	void Font::setFontResolution(uint32 ttfResolution)
	{
		resolution = ttfResolution;
	}

	uint32 Font::getFontResolution()
	{
		return resolution;
	}

	void Font::setCacheSize(uint32 cacheSize)
	{
		cacheBufferSize = cacheSize;
	}

	uint32 Font::getCacheBufferSize()
	{
		return cacheBufferSize;		
	}

	const CharGlyph& Font::getCharGlyph(uint32 codePoint)
	{

		std::deque<CachedChar>::iterator it = cacheCharQueue.begin();
		for (;it < cacheCharQueue.end(); ++it){
			CachedChar tmpCacheChar = (*it);
			if (tmpCacheChar.codePoint == codePoint){

				charGlyph = tmpCacheChar.charGlyph;

				if (it != cacheCharQueue.begin()){
					//move the char to deque's front
					cacheCharQueue.erase(it);

					cacheCharQueue.push_front(tmpCacheChar);				
				}

				return charGlyph;
			}		
		}

		try{
			addCharGlyph(codePoint);		
		}catch(...){
			getCharGlyph(CODEPOINT_QUAD); //the char glyph is quad
		}

		const CachedChar& tmpCacheChar = cacheCharQueue.front();

		charGlyph = tmpCacheChar.charGlyph;
		
		return charGlyph;
	}

	int Font::getMaxCharWidth()
	{
		return  maxCharWidth;
	}

	Texture* Font::getFontTexture()
	{
		return texCache;
	}

	const ShaderParams& Font::getFontExtParams()
	{
		return shaderParams;
	}


	void Font::addCharGlyph(uint32 codePoint)
	{
		assert(cacheCharQueue.size() <= cacheBufferSize);

		FT_Error ftResult;

		ftResult = FT_Load_Char( fontResource->face, codePoint, FT_LOAD_RENDER );

		if (ftResult){//装在字模失败
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "FT_Load_Char", "Font::addCharGlyph");
		}

		if (fontResource->face->glyph->bitmap.rows > maxCharWidth || fontResource->face->glyph->bitmap.width > maxCharWidth){
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "char size overcome maxCharWidth!", "Font::addCharGlyph");
		}

		//如果cache队列已经满了
		if (cacheCharQueue.size() == cacheBufferSize){
			//将 willBeFillCachedPos 指向最不常用的字
			willBeFillCachedPos = cacheCharQueue.back();

			cacheCharQueue.pop_back();
		}

		//clear char glyph base
		Image clearImg;

		clearImg.create(maxCharWidth, maxCharWidth, HPF_A8R8G8B8);

		texCache->upload(clearImg, willBeFillCachedPos.x, willBeFillCachedPos.y);

		uint8* buffer = fontResource->face->glyph->bitmap.buffer;

		int char_width = 0;
		int char_height = 0;

		if (!buffer){//char glyph space
			
			char_height = maxCharWidth;

			char_width  = fontResource->face->glyph->metrics.horiAdvance / PER_WIDTH;
		
		}else{
			Image img;

			img.create(maxCharWidth, maxCharWidth, HPF_A8R8G8B8);
			
			for (int count1 = 0; count1 < fontResource->face->glyph->bitmap.rows; ++count1){
				uint8* destBuffer = (uint8*)img.getImageData() + img.getRowStride() * count1;
				for (int count2 = 0; count2 < fontResource->face->glyph->bitmap.width; ++count2){
					*(destBuffer)   = *(buffer);//B
					*(destBuffer+1) = *(buffer);//G
					*(destBuffer+2) = *(buffer);//R
					*(destBuffer+3) = *(buffer);//A
					destBuffer += 4;
					buffer += 1;
				}
			}

			char_width = fontResource->face->glyph->bitmap.width;

			char_height= fontResource->face->glyph->bitmap.rows;

			//upload char glyph
			texCache->upload(img, willBeFillCachedPos.x, willBeFillCachedPos.y);
		}

		willBeFillCachedPos.codePoint = codePoint;
		willBeFillCachedPos.charGlyph.recGlyph.minX = (float)(willBeFillCachedPos.x) / (float)texCache->getWidth();
		willBeFillCachedPos.charGlyph.recGlyph.minY = (float)(willBeFillCachedPos.y) / (float)texCache->getHeight();
		willBeFillCachedPos.charGlyph.recGlyph.maxX = (float)(willBeFillCachedPos.x + char_width)  / (float)texCache->getWidth();
		willBeFillCachedPos.charGlyph.recGlyph.maxY = (float)(willBeFillCachedPos.y + char_height) / (float)texCache->getHeight();
		willBeFillCachedPos.charGlyph.baselineX = (float)(fontResource->face->glyph->metrics.horiBearingY)    / PER_WIDTH;
		willBeFillCachedPos.charGlyph.bear_left = (float)(fontResource->face->glyph->metrics.horiBearingX)    / PER_WIDTH;
		willBeFillCachedPos.charGlyph.bear_advanceX = (float)(fontResource->face->glyph->metrics.horiAdvance) / PER_WIDTH;
		willBeFillCachedPos.charGlyph.baselineY = (float)(fontResource->face->glyph->metrics.vertBearingX)    / PER_WIDTH;
		willBeFillCachedPos.charGlyph.bear_top = (float)(fontResource->face->glyph->metrics.vertBearingY)     / PER_WIDTH;
		willBeFillCachedPos.charGlyph.bear_advanceY = (float)(fontResource->face->glyph->metrics.vertAdvance) / PER_WIDTH;

		//将 新加入的到cache的字放到队列的首位
		cacheCharQueue.push_front(willBeFillCachedPos);

		if (cacheCharQueue.size() < cacheBufferSize){//还有空位
			willBeFillCachedPos.x = (cacheCharQueue.size() % numCharPerLine) * maxCharWidth;
			willBeFillCachedPos.y = (cacheCharQueue.size() / numCharPerLine) * maxCharWidth;
		}

	}

	void Font::advanceFillCache(uint32 codePointBegin, uint32 codePointEnd)
	{
		for (uint32 count = codePointBegin; count <= codePointEnd; ++count){
			try{
				addCharGlyph(count);
			}catch(...){
				getCharGlyph(CODEPOINT_QUAD);
			}
		}
	}

}
