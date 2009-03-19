#include "PCH.h"
#include "Font.h"
#include "SystemManager.h"

#define PER_WIDTH 64
#define CODEPOINT_QUAD L'A' + 63

namespace hare_graphics
{
	Font::Font()
		:bInitalize(false)
	{
	}

	Font::Font(const String& ttfName, f32 ttfSize, u32 ttfResolution, u32 cacheSize)
		:fontName(ttfName)
		,fontSize(ttfSize)
		,resolution(ttfResolution)
		,cacheBufferSize(cacheSize)
		,bInitalize(false)
	{
		postLoaded();
	}

	Font::~Font()
	{
		destoryResouse();
	}

	void Font::initalzeResouse()
	{
		shaderParams.AlphaBlendEnable = true;
		shaderParams.AlphaTestEnable  = false;

		String tmpExt;
		String fontBaseName;
		StringUtil::splitFilename(fontName, fontBaseName, tmpExt);
		StringUtil::toLowerCase(tmpExt);
		assert(tmpExt == "ttf");

		FT_Error ftResult = FT_Init_FreeType( &ftLibrary );
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not init FreeType library!", "Font::Font FT_Init_FreeType");

		bInitalize = true;
		/*文件系统打开文件将数据读进input
		*/
		FileSystem* fs = getFileSystem();
		FileHandle fh = fs->openFile(fontName, FM_Read);

		if (!fh)
			HARE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "Could not open FontFile!", "Font::Font fs->openFile");

		int size = fs->size(fh);
		input.allocate(size);
		fs->readFile(fh, (char*)input.getData(), size, 1);
		fs->closeFile(fh);

		ftResult = FT_New_Memory_Face(ftLibrary, (FT_Byte*)input.getData(), (FT_Long)input.getSize(), 0, &face);
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not open font face!", "Font::Font FT_New_Memory_Face");

		FT_F26Dot6  ftSize = (FT_F26Dot6)(fontSize * (1 << 6));
		ftResult = FT_Set_Char_Size( face, ftSize, 0, resolution, resolution );
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not set char size!", "Font::Font FT_Set_Char_Size");

		int max_height = 0, max_width = 0;

		//随即抽取几个中文字符选出最大的宽度和空隙
		wchar_t ch = L'赢';
		for (wchar_t count = ch - 50; count < ch + 50; ++count){
			ftResult = FT_Load_Char( face, count, FT_LOAD_RENDER);
			if (ftResult)
				continue;

			if (face->glyph->bitmap.rows > max_height)
				max_height = face->glyph->bitmap.rows;

			if (face->glyph->bitmap.width > max_width)
				max_width = face->glyph->bitmap.width;		
		}

		ch = L'A';
		for (wchar_t count = ch; count < ch + 63; ++count){
			ftResult = FT_Load_Char( face, count, FT_LOAD_RENDER);
			if (ftResult)
				continue;

			if (face->glyph->bitmap.rows > max_height)
				max_height = face->glyph->bitmap.rows;

			if (face->glyph->bitmap.width > max_width)
				max_width = face->glyph->bitmap.width;		
		}

		//最大字符宽度
		maxCharWidth = max(max_width, max_height);

		//纹理尺寸
		u32 texSize = MathUtil::firstPO2From(sqrt((f32)cacheBufferSize) * maxCharWidth);

		//纹理每行装的字符数
		numCharPerLine = texSize / maxCharWidth;

		//字符实际缓冲大小
		cacheBufferSize = numCharPerLine * numCharPerLine; 

		texCache = SystemManager::getSingletonPtr()->createTexture();

		texCache->createRenderToTex(texSize, texSize, HPF_A8R8G8B8);

		ch = L'A';
		advanceFillCache(ch, ch + 63);
	}

	void Font::destoryResouse()
	{
		if (bInitalize){
			FT_Done_FreeType(ftLibrary);
			bInitalize = false;
		}
	}

	void Font::postLoaded()
	{
		destoryResouse();

		initalzeResouse();
	}

	void Font::setFontFileName(const String& ttfName)
	{
		String fontName = ttfName;
	}

	const String& Font::getFontFileName()
	{
		return fontName;
	}

	void Font::setFontSize(f32 ttfSize)
	{
		f32 fontSize = ttfSize;
	}

	f32 Font::getFontSize()
	{
		return fontSize;
	}

	void Font::setFontResolution(u32 ttfResolution)
	{
		resolution = ttfResolution;
	}

	u32 Font::getFontResolution()
	{
		return resolution;
	}

	void Font::setCacheSize(u32 cacheSize)
	{
		cacheBufferSize = cacheSize;
	}

	u32 Font::getCacheBufferSize()
	{
		return cacheBufferSize;		
	}

	const CharGlyph& Font::getCharGlyph(u32 codePoint)
	{

		std::deque<CachedChar>::iterator it = cacheCharQueue.begin();
		for (;it < cacheCharQueue.end(); ++it){
			CachedChar tmpCacheChar = (*it);
			if (tmpCacheChar.codePoint == codePoint){

				charGlyph.baselineX     = tmpCacheChar.baselineX;
				charGlyph.bear_left     = tmpCacheChar.bear_left;
				charGlyph.bear_advanceX = tmpCacheChar.bear_advanceX;
				charGlyph.baselineY     = tmpCacheChar.baselineY;
				charGlyph.bear_top      = tmpCacheChar.bear_top;
				charGlyph.bear_advanceY = tmpCacheChar.bear_advanceY;

				charGlyph.recGlyph = tmpCacheChar.recGlyph;

				//move the char to deque's front
				cacheCharQueue.erase(it);

				cacheCharQueue.push_front(tmpCacheChar);

				return charGlyph;
			}		
		}

		try{
			addCharGlyph(codePoint);		
		}catch(...){
			addCharGlyph(CODEPOINT_QUAD); //the char glyph is quad
		}

		const CachedChar& tmpCacheChar = cacheCharQueue.front();

		charGlyph.baselineX     = tmpCacheChar.baselineX;
		charGlyph.bear_left     = tmpCacheChar.bear_left;
		charGlyph.bear_advanceX = tmpCacheChar.bear_advanceX;
		charGlyph.baselineY     = tmpCacheChar.baselineY;
		charGlyph.bear_top      = tmpCacheChar.bear_top;
		charGlyph.bear_advanceY = tmpCacheChar.bear_advanceY;

		charGlyph.recGlyph = tmpCacheChar.recGlyph;
		
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


	void Font::addCharGlyph(u32 codePoint)
	{
		assert(cacheCharQueue.size() <= cacheBufferSize);

		FT_Error ftResult;

		ftResult = FT_Load_Char( face, codePoint, FT_LOAD_RENDER );

		if (ftResult){//装在字模失败
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "FT_Load_Char", "Font::addCharGlyph");
		}

		if (face->glyph->bitmap.rows > maxCharWidth || face->glyph->bitmap.width > maxCharWidth){
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "char size overcome maxCharWidth!", "Font::addCharGlyph");
		}

		//clear char glyph base
		Image clearImg;

		clearImg.create(maxCharWidth, maxCharWidth, HPF_A8R8G8B8);

		texCache->upload(clearImg, willBeFillCachedPos.x, willBeFillCachedPos.y);

		u8* buffer = face->glyph->bitmap.buffer;

		int char_width = 0;
		int char_height = 0;

		if (!buffer){//char glyph space
			
			char_height = maxCharWidth;

			char_width  = face->glyph->metrics.horiAdvance / PER_WIDTH;
		
		}else{
			Image img;

			img.create(maxCharWidth, maxCharWidth, HPF_A8R8G8B8);
			
			for (int count1 = 0; count1 < face->glyph->bitmap.rows; ++count1){
				u8* destBuffer = (u8*)img.getImageData() + img.getRowStride() * count1;
				for (int count2 = 0; count2 < face->glyph->bitmap.width; ++count2){
					*(destBuffer)   = *(buffer);//B
					*(destBuffer+1) = *(buffer);//G
					*(destBuffer+2) = *(buffer);//R
					*(destBuffer+3) = *(buffer);//A
					destBuffer += 4;
					buffer += 1;
				}
			}

			char_width = face->glyph->bitmap.width;

			char_height= face->glyph->bitmap.rows;

			//upload char glyph
			texCache->upload(img, willBeFillCachedPos.x, willBeFillCachedPos.y);
		}

		if (cacheCharQueue.size() == cacheBufferSize){
			cacheCharQueue.pop_back();
		}

		willBeFillCachedPos.codePoint = codePoint;
		willBeFillCachedPos.recGlyph.minX = (f32)(willBeFillCachedPos.x) / (f32)texCache->getWidth();
		willBeFillCachedPos.recGlyph.minY = (f32)(willBeFillCachedPos.y) / (f32)texCache->getHeight();
		willBeFillCachedPos.recGlyph.maxX = (f32)(willBeFillCachedPos.x + char_width)  / (f32)texCache->getWidth();
		willBeFillCachedPos.recGlyph.maxY = (f32)(willBeFillCachedPos.y + char_height) / (f32)texCache->getHeight();
		willBeFillCachedPos.baselineX = (f32)(face->glyph->metrics.horiBearingY)    / PER_WIDTH;
		willBeFillCachedPos.bear_left = (f32)(face->glyph->metrics.horiBearingX)    / PER_WIDTH;
		willBeFillCachedPos.bear_advanceX = (f32)(face->glyph->metrics.horiAdvance) / PER_WIDTH;
		willBeFillCachedPos.baselineY = (f32)(face->glyph->metrics.vertBearingX)    / PER_WIDTH;
		willBeFillCachedPos.bear_top = (f32)(face->glyph->metrics.vertBearingY)     / PER_WIDTH;
		willBeFillCachedPos.bear_advanceY = (f32)(face->glyph->metrics.vertAdvance) / PER_WIDTH;

		//将 新加入的到cache的字放到队列的首位
		cacheCharQueue.push_front(willBeFillCachedPos);

		//如果cache队列已经满了
		if (cacheCharQueue.size() == cacheBufferSize){
			//将 willBeFillCachedPos 指向最不常用的字
			willBeFillCachedPos = cacheCharQueue.back();
		}else{//还有空位
			willBeFillCachedPos.x = (cacheCharQueue.size() % numCharPerLine) * maxCharWidth;
			willBeFillCachedPos.y = (cacheCharQueue.size() / numCharPerLine) * maxCharWidth;
		}

	}

	void Font::advanceFillCache(u32 codePointBegin, u32 codePointEnd)
	{
		for (u32 count = codePointBegin; count <= codePointEnd; ++count){
			try{
				addCharGlyph(count);
			}catch(...){
				addCharGlyph(CODEPOINT_QUAD);
			}
		}
	}

}