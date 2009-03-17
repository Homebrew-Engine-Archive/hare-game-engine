#include "PCH.h"
#include "Font.h"
#include "SystemManager.h"

#define PER_WIDTH (26.6f * 2)
#define CODEPOINT_QUAD L'A' + 63

namespace hare_graphics
{
	Font::Font(const String& ttfName, f32 ttfSize, u32 ttfResolution, u32 cacheSize)
		:fontSize(ttfSize)
	{
		String tmpExt;
		StringUtil::splitFilename(ttfName, fontName, tmpExt);
		StringUtil::toLowerCase(tmpExt);
		assert(tmpExt == "ttf");

		FT_Error ftResult = FT_Init_FreeType( &ftLibrary );
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not init FreeType library!", "Font::Font FT_Init_FreeType");

		/*文件系统打开文件将数据读进input
		*/
		FileSystem* fs = getFileSystem();
		FileHandle fh = fs->openFile(ttfName, FM_Read);

		if (!fh)
			HARE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "Could not open FontFile!", "Font::Font fs->openFile");

		int size = fs->size(fh);
		input.allocate(size);
		fs->readFile(fh, (char*)input.getData(), size, 1);
		fs->closeFile(fh);
		
		ftResult = FT_New_Memory_Face(ftLibrary, (FT_Byte*)input.getData(), (FT_Long)input.getSize(), 0, &face);
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not open font face!", "Font::Font FT_New_Memory_Face");

		FT_F26Dot6  ftSize = (FT_F26Dot6)(ttfSize * (1 << 6));
		ftResult = FT_Set_Char_Size( face, ftSize, 0, ttfResolution, ttfResolution );
		if (ftResult)
			HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Could not set char size!", "Font::Font FT_Set_Char_Size");

		int max_height = 0, max_width = 0, max_baseline_top = 0, max_baseline_bottom = 0;
		
		//随即抽取几个中文字符选出最大的宽度和空隙
		wchar_t ch = L'赢';
		for (wchar_t count = ch - 50; count < ch + 50; ++count){
			assert(0 == FT_Load_Char( face, count, FT_LOAD_RENDER));

			if (face->glyph->metrics.horiBearingY + face->glyph->metrics.vertBearingY > max_baseline_top)
				max_baseline_top = face->glyph->metrics.horiBearingY + face->glyph->metrics.vertBearingY;

			if (face->glyph->metrics.vertAdvance - (face->glyph->metrics.horiBearingY + face->glyph->metrics.vertBearingY) > max_baseline_bottom)
				max_baseline_bottom = face->glyph->metrics.vertAdvance - (face->glyph->metrics.horiBearingY + face->glyph->metrics.vertBearingY);

			if (face->glyph->metrics.horiAdvance > max_width)
				max_width = face->glyph->metrics.horiAdvance;		
		}

		baseline = max_baseline_top;

		max_height = (max_baseline_top + max_baseline_bottom) / PER_WIDTH;

		max_width /= PER_WIDTH;
		//最大字符宽度
		maxCharWidth = max(max_width, max_height);

		//纹理尺寸
		u32 texSize = MathUtil::firstPO2From(sqrt((f32)cacheSize) * maxCharWidth);

		//纹理每行装的字符数
		numCharPerLine = texSize / maxCharWidth;

		//字符实际缓冲大小
		cacheBufferSize = numCharPerLine * numCharPerLine; 

		texCache = SystemManager::getSingletonPtr()->createTexture();

		texCache->createRenderToTex(texSize, texSize, HPF_A8R8G8B8);

		ch = L'A';
		advanceFillCache(ch, ch + 63);
			
	}

	Font::~Font()
	{
		FT_Done_FreeType(ftLibrary);
	}

	const CharGlyph& Font::getCharGlyph(u32 codePoint)
	{

		std::deque<CachedChar>::iterator it = cacheCharQueue.begin();
		for (;it < cacheCharQueue.end(); ++it){
			const CachedChar& tmpCacheChar = (*it);
			if (tmpCacheChar.codePoint == codePoint){
				charGlyph.recGlyph = tmpCacheChar.recGlyph;
				charGlyph.texGlyph = texCache;

				//move the char to deque's front
				cacheCharQueue.push_front(tmpCacheChar);

				cacheCharQueue.erase(it);

				return charGlyph;
			}		
		}

		try{
			addCharGlyph(codePoint);		
		}catch(...){
			addCharGlyph(CODEPOINT_QUAD); //the char glyph is quad
		}

		const CachedChar& tmpCacheChar = cacheCharQueue.front();
		charGlyph.recGlyph = tmpCacheChar.recGlyph;
		charGlyph.texGlyph = texCache;
		
		return charGlyph;
	}

	const String& Font::getFontName()
	{
		return fontName;
	}

	f32 Font::getFonSize()
	{
		return fontSize;
	}

	int Font::getMaxCharWidth()
	{
		return  maxCharWidth;
	}

	u32 Font::getCacheBufferSize()
	{
		return cacheBufferSize;		
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

		f32 left_bear = 0;

		f32 right_boundary = 0;

		f32 top_bear = 0;

		u8* buffer = face->glyph->bitmap.buffer;

		if (!buffer){//char glyph space
			
			right_boundary = (f32)(face->glyph->metrics.horiAdvance) / PER_WIDTH;
		
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

			left_bear = (face->glyph->metrics.horiBearingX > 0 ? face->glyph->metrics.horiBearingX : 0) / PER_WIDTH;

			right_boundary = (f32)(face->glyph->metrics.horiAdvance) / PER_WIDTH;

			top_bear  = (baseline - ((face->glyph->metrics.horiBearingY > 0 ? face->glyph->metrics.horiBearingY : 0))) / PER_WIDTH;

			//upload char glyph
			texCache->upload(img, willBeFillCachedPos.x + left_bear, willBeFillCachedPos.y + top_bear);
		}

		if (cacheCharQueue.size() == cacheBufferSize){
			cacheCharQueue.pop_back();
		}

		willBeFillCachedPos.codePoint = codePoint;
		willBeFillCachedPos.recGlyph.minX = (f32)(willBeFillCachedPos.x) / (f32)texCache->getWidth();
		willBeFillCachedPos.recGlyph.minY = (f32)(willBeFillCachedPos.y) / (f32)texCache->getHeight();
		willBeFillCachedPos.recGlyph.maxX = (f32)(willBeFillCachedPos.x + right_boundary) / (f32)texCache->getWidth();
		willBeFillCachedPos.recGlyph.maxY = (f32)(willBeFillCachedPos.y + maxCharWidth) / (f32)texCache->getHeight();

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