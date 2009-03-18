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
		if (!font)
			return;

		WString wstr = StringUtil::fromUTF8(text);
		Shader::Ptr shader = new SimpleShader;
		TextureMtrl::Ptr texMtrl = new TextureMtrl;

		f32 layout_x = 0;
		for (u32 count = 0; count < wstr.size(); ++count){
			CharGlyph charGlyph = font->getCharGlyph(wstr[count]);
			texMtrl->setTexture(charGlyph.texGlyph);
			shader->setMaterial(texMtrl);

			Quad quad;
			quad.setShader(shader);
			quad.moveTo(x + layout_x, y);
			f32 w = (charGlyph.recGlyph.maxX - charGlyph.recGlyph.minX) * charGlyph.texGlyph->getWidth();
			layout_x += w;
			quad.setWidth(w);
			quad.setHeight((charGlyph.recGlyph.maxY - charGlyph.recGlyph.minY) * charGlyph.texGlyph->getHeight());
			texMtrl->setUV(charGlyph.recGlyph.minX, charGlyph.recGlyph.minY, charGlyph.recGlyph.maxX, charGlyph.recGlyph.maxY);
			RenderSystem::getSingletonPtr()->render(&quad);			
		}

		CharGlyph charGlyph = font->getCharGlyph(wstr[0]);

		Quad quad2;
		quad2.setShader(shader);
		quad2.moveTo(150,150);
		quad2.setWidth(charGlyph.texGlyph->getWidth());
		quad2.setHeight(charGlyph.texGlyph->getHeight());
		texMtrl->setUV(0,0,1,1);
		RenderSystem::getSingletonPtr()->render(&quad2);	

	}

	void Canvas::drawImage(int x, int y, Shader* shader, f32 z)
	{
		if (!shader)
			return;

		Material* mtrl = shader->getMaterial();
		if (!mtrl)
			return;

		mtrl->frameMove();

		Quad quad;
		quad.setShader(shader);
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

	void Canvas::drawImage(const Rect<int>& rect, Shader* shader, float rot, f32 z)
	{
		if (!shader)
			return;

		Material* mtrl = shader->getMaterial();

		if (!mtrl)
			return;

		mtrl->frameMove();

		Quad quad;
		quad.setShader(shader);
		quad.moveTo((f32)rect.minX, (f32)rect.minY);
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