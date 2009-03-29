#include "PCH.h"
#include "Canvas.h"
#include "Line.h"
#include "Quad.h"
#include "RenderSystem.h"
#include "Font.h"


namespace hare_graphics
{
	HARE_IMPLEMENT_SINGLETON(Canvas)

	Canvas::Canvas()
		:font(NULL)
	{
		
	}

	Canvas::~Canvas()
	{

	}

	void Canvas::drawLine(int x1, int y1, int x2, int y2, u32 color, f32 z)
	{
		Line line;
		line.set(x1, y1, x2, y2, color, z);
		RenderSystem::getSingletonPtr()->render(&line);
	}

	void Canvas::drawRect(int l, int t, int r, int b, u32 color, f32 z)
	{
		Line line;
		line.set(l, t, r, t, color, z);
		RenderSystem::getSingletonPtr()->render(&line);		
		line.set(r, t, r, b, color, z);
		RenderSystem::getSingletonPtr()->render(&line);	
		line.set(l, b, r, b, color, z);
		RenderSystem::getSingletonPtr()->render(&line);	
		line.set(l, t, l, b, color, z);
		RenderSystem::getSingletonPtr()->render(&line);	
	}

	void Canvas::drawText(int x, int y, const String& text)
	{
		if (!font || text.empty())
			return;

		WString wstr = StringUtil::fromUTF8(text);
		TextureMtrl* texMtrl = new TextureMtrl;
		Shader::Ptr shader = new ParticleShader;
		texMtrl->setTexture(font->getFontTexture());
		//shader->setShaderParams(font->getFontExtParams());
		shader->setMaterial(texMtrl);
		f32 layout_x = (f32)x;
		f32 layout_y = 0;
		
		for (u32 batchCount = 0; batchCount <= wstr.size() / font->getCacheBufferSize(); ++batchCount){
			
			u32 batchNum;
			
			if (batchCount == wstr.size() / font->getCacheBufferSize()){
				batchNum = wstr.size() % font->getCacheBufferSize();
			}else{
				batchNum = font->getCacheBufferSize();
			}

			for (u32 count = batchCount * font->getCacheBufferSize(); count < batchCount * font->getCacheBufferSize() + batchNum; ++count){
				CharGlyph charGlyph = font->getCharGlyph(wstr[count]);

				layout_x += charGlyph.bear_left;
				layout_y = y - charGlyph.baselineX;



				Quad quad;
				quad.setMaterial(shader);
				quad.moveTo(layout_x, layout_y);

				layout_x += charGlyph.bear_advanceX - charGlyph.bear_left;

				quad.setWidth((charGlyph.recGlyph.maxX - charGlyph.recGlyph.minX) * font->getFontTexture()->getWidth());
				quad.setHeight((charGlyph.recGlyph.maxY - charGlyph.recGlyph.minY) * font->getFontTexture()->getHeight());
				quad.setTextureUVMap(charGlyph.recGlyph);
				RenderSystem::getSingletonPtr()->render(&quad);		
			}		
			RenderSystem::getSingletonPtr()->render();
		}

		//Quad quad2;
		//quad2.setShader(shader);
		//quad2.moveTo(150,150);
		//quad2.setWidth(font->getFontTexture()->getWidth());
		//quad2.setHeight(font->getFontTexture()->getHeight());
		//texMtrl->setUV(0,0,1,1);
		//RenderSystem::getSingletonPtr()->render(&quad2);	

	}

	void Canvas::drawImage(int x, int y, Material* mtrl, f32 z)
	{
		if (!mtrl)
			return;

		mtrl->frameMove();

		Quad quad;
		quad.setMaterial(mtrl);
		quad.moveTo((f32)x, (f32)y);
		quad.setDepth(z);
		TextureMtrl* texMtrl = mtrl->getTextureMtrl();
		if (texMtrl){
			Texture* tex = texMtrl->getTexture();
			if (tex){
				quad.setWidth((f32)tex->getWidth());
				quad.setHeight((f32)tex->getHeight());
			}
		}
		RenderSystem::getSingletonPtr()->render(&quad);	
	}

	void Canvas::drawImage(const RectF& rect, Material* mtrl, f32 rot, f32 z)
	{
		if (!mtrl)
			return;

		mtrl->frameMove();

		Quad quad;
		quad.setMaterial(mtrl);
		quad.moveTo(rect.minX, rect.minY);
		quad.setDepth(z);
		quad.setWidth(rect.width());
		quad.setHeight(rect.height());
		RenderSystem::getSingletonPtr()->render(&quad);		
	}

	void Canvas::setFont(Font* f)
	{
		font = f;
	}

	Canvas* getCanvas()
	{
		return Canvas::getSingletonPtr();
	}
}