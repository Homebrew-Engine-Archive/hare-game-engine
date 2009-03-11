#include "PCH.h"
#include "Canvas.h"
#include "Line.h"
#include "Quad.h"
#include "RenderSystem.h"


namespace hare_graphics
{
	HARE_IMPLEMENT_SINGLETON(Canvas)

	Canvas::Canvas()
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

	Canvas* getCanvas()
	{
		static Canvas canvas;
		return &canvas;
	}
}