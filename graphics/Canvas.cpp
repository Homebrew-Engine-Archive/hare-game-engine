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
		,color(-1)
		,z(0)
	{
		
	}

	Canvas::~Canvas()
	{

	}

	void Canvas::drawLine(int x1, int y1, int x2, int y2)
	{
		Line line;
		line.set(x1, y1, x2, y2, color, z);
		RenderSystem::getSingletonPtr()->render(&line);
	}

	void Canvas::drawRect(int l, int t, int r, int b)
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
				quad.setColor(color);
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

	void Canvas::drawImage(int x, int y, Material* mtrl, const RectF& uvRect)
	{
		if (!mtrl)
			return;

		mtrl->frameMove();

		Quad quad;
		quad.setMaterial(mtrl);
		quad.moveTo((f32)x, (f32)y);
		quad.setDepth(z);
		quad.setColor(color);
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

	void Canvas::drawImage(const RectF& rect, Material* mtrl, const RectF& uvRect)
	{
		if (!mtrl)
			return;

		mtrl->frameMove();

		Quad quad;
		quad.setMaterial(mtrl);
		quad.moveTo(rect.minX, rect.minY);
		quad.setDepth(z);
		quad.setColor(color);
		quad.setWidth(rect.width());
		quad.setHeight(rect.height());
		RenderSystem::getSingletonPtr()->render(&quad);		
	}

	void Canvas::drawImage(const RectF& rect, Material* mtrl, f32 rot, const RectF& uvRect, const PointF& center)
	{
		if (!mtrl)
			return;

		mtrl->frameMove();

		f32 width  = (f32)mtrl->getTextureMtrl()->getTexture()->getWidth();
		f32 height = (f32)mtrl->getTextureMtrl()->getTexture()->getHeight();
		f32 origoX = center.x * width;
		f32 origoY = center.y * height;
		f32 hscale = rect.width() / width;
		f32 vscale = rect.height()/ height;

		f32 tx1, ty1, tx2, ty2;
		f32 sint, cost;

		if(vscale == 0.0f) vscale = hscale;

		tx1 = -origoX * hscale;
		ty1 = -origoY * vscale;
		tx2 = (width  - origoX) * hscale;
		ty2 = (height - origoY) * vscale;

		Quad quad;
		quad.setMaterial(mtrl);
		quad.setDepth(z);
		quad.setColor(color);

		if(rot != 0.0f)
		{
			cost = MathUtil::cosf(rot);
			sint = MathUtil::sinf(rot);

			quad.buffer[0].x = tx1*cost - ty1*sint + rect.minX;
			quad.buffer[0].y = tx1*sint + ty1*cost + rect.minY;	

			quad.buffer[1].x = tx2*cost - ty1*sint + rect.minX;
			quad.buffer[1].y = tx2*sint + ty1*cost + rect.minY;	

			quad.buffer[2].x = tx2*cost - ty2*sint + rect.minX;
			quad.buffer[2].y = tx2*sint + ty2*cost + rect.minY;	

			quad.buffer[3].x = tx1*cost - ty2*sint + rect.minX;
			quad.buffer[3].y = tx1*sint + ty2*cost + rect.minY;	
		}
		else
		{
			quad.buffer[0].x = tx1 + rect.minX; 
			quad.buffer[0].y = ty1 + rect.minY;
			quad.buffer[1].x = tx2 + rect.minX; 
			quad.buffer[1].y = ty1 + rect.minY;
			quad.buffer[2].x = tx2 + rect.minX; 
			quad.buffer[2].y = ty2 + rect.minY;
			quad.buffer[3].x = tx1 + rect.minX; 
			quad.buffer[3].y = ty2 + rect.minY;
		}

		RenderSystem::getSingletonPtr()->render(&quad);		

	}

	void Canvas::setFont(Font* f)
	{
		font = f;
	}

	void Canvas::setColor(u32 c)
	{
		color = c;
	}

	void Canvas::setZ(f32 z)
	{
		this->z = z;
	}

	u32  Canvas::getColor()
	{
		return color;
	}

	f32  Canvas::getZ()
	{
		return z;
	}

	Canvas* getCanvas()
	{
		return Canvas::getSingletonPtr();
	}
}