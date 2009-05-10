//***************************************************************
//  File:    Canvas.cpp
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "Canvas.h"
#include "Line.h"
#include "Quad.h"
#include "RenderSystem.h"
#include "Font.h"


namespace hare
{
	HARE_IMPLEMENT_SINGLETON(Canvas)

	Canvas::Canvas()
		:font(NULL)
		,z(0)
        ,color(-1)
	{
	}

	void Canvas::drawLine(float x1, float y1, float x2, float y2)
	{
		Line line;
		line.set(x1, y1, x2, y2, color, z);
		RenderSystem::getSingletonPtr()->render(&line);
	}

	void Canvas::drawRect(float l, float t, float r, float b)
	{
		Line line;
		line.set(l, t, r, t, color, z);
		RenderSystem::getSingletonPtr()->render(&line);
		line.set(r, t, r, b, color, z);
		RenderSystem::getSingletonPtr()->render(&line);
		line.set(r, b, l, b, color, z);
		RenderSystem::getSingletonPtr()->render(&line);
		line.set(l, b, l, t, color, z);
		RenderSystem::getSingletonPtr()->render(&line);
	}

    void Canvas::drawRectFill(float l, float t, float r, float b)
    {
        Quad quad;
        quad.moveTo(l, t);
        quad.setDepth(z);
        quad.setColor(color);
        quad.setWidth(r - l);
        quad.setHeight(b - t);
        RenderSystem::getSingletonPtr()->render(&quad);
    }

	void Canvas::drawText(float x, float y, const String& text)
	{
		if (!font || text.empty())
			return;

		WString wstr = StringUtil::fromUTF8(text);
		TextureMtrl* texMtrl = new TextureMtrl;
		Shader::Ptr shader = new ParticleShader;
		texMtrl->setTexture(font->getFontTexture());
		//shader->setShaderParams(font->getFontExtParams());
		shader->setMaterial(texMtrl);
		float layout_x = x;
		float layout_y = 0;

		for (uint32 batchCount = 0; batchCount <= wstr.size() / font->getCacheBufferSize(); ++batchCount){

			uint32 batchNum;

			if (batchCount == wstr.size() / font->getCacheBufferSize()){
				batchNum = wstr.size() % font->getCacheBufferSize();
			}else{
				batchNum = font->getCacheBufferSize();
			}

			for (uint32 count = batchCount * font->getCacheBufferSize(); count < batchCount * font->getCacheBufferSize() + batchNum; ++count){
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
				quad.setDepth(z);
				RenderSystem::getSingletonPtr()->render(&quad);
			}
			RenderSystem::getSingletonPtr()->render();
		}

		//Quad quad2;
		//quad2.setMaterial(shader);
		//quad2.moveTo(150,150);
		//quad2.setWidth((float)font->getFontTexture()->getWidth());
		//quad2.setHeight((float)font->getFontTexture()->getHeight());
		////texMtrl->setUV(0,0,1,1);
		//RenderSystem::getSingletonPtr()->render(&quad2);	

	}

	void Canvas::drawImage(float x, float y, Material* mtrl, const RectF& uvRect)
	{
		if (!mtrl)
			return;

		mtrl->frameMove();

		Quad quad;
		quad.setMaterial(mtrl);
		quad.moveTo(x, y);
		quad.setDepth(z);
		quad.setColor(color);
		quad.setTextureUVMap(uvRect);
		TextureMtrl* texMtrl = mtrl->getTextureMtrl();
		if (texMtrl){
			Texture* tex = texMtrl->getTexture();
			if (tex){
				quad.setWidth((float)tex->getWidth());
				quad.setHeight((float)tex->getHeight());
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
        quad.setTextureUVMap(uvRect);
		RenderSystem::getSingletonPtr()->render(&quad);
	}

	void Canvas::drawImage(const RectF& rect, Material* mtrl, float rot, const RectF& uvRect, const PointF& center)
	{
		if (!mtrl)
			return;

		mtrl->frameMove();

		float width  = (float)mtrl->getTextureMtrl()->getTexture()->getWidth();
		float height = (float)mtrl->getTextureMtrl()->getTexture()->getHeight();
		float origoX = center.x * width;
		float origoY = center.y * height;
		float hscale = rect.width() / width;
		float vscale = rect.height()/ height;

		float tx1, ty1, tx2, ty2;
		float sint, cost;

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
        quad.setTextureUVMap(uvRect);
		RenderSystem::getSingletonPtr()->render(&quad);
	}

	void Canvas::setFont(Font* f)
	{
		font = f;
	}

	Font* Canvas::getFont()
	{
		return font;
	}

	void Canvas::setColor(uint32 c)
	{
		color = c;
	}

	void Canvas::setZ(float z)
	{
		this->z = z;
	}

	uint32  Canvas::getColor()
	{
		return color;
	}

	float  Canvas::getZ()
	{
		return z;
	}

	Canvas* getCanvas()
	{
		return Canvas::getSingletonPtr();
	}
}
