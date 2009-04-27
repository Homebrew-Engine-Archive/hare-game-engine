#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "Font.h"

namespace hare
{
	class Material;

	class GRAPHICS_API Canvas : public Singleton<Canvas>
	{
		HARE_DECLARE_SINGLETON(Canvas)
	public:
		Canvas();

		void drawLine(float x1, float y1, float x2, float y2);

        void drawRect(float l, float t, float r, float b);
        void drawRectFill(float l, float t, float r, float b);

		void drawText(int x, int y, const String& text);

		void drawImage(int x, int y, Material* mtrl, const RectF& uvRect = RectF(0,0,1,1));

		void drawImage(const RectF& rect, Material* mtrl, const RectF& uvRect = RectF(0,0,1,1));

		void drawImage(const RectF& rect, Material* mtrl, float rot, const RectF& uvRect = RectF(0,0,1,1), const PointF& center = PointF(0.5f, 0.5f));

		void setFont(Font* f);

		Font* getFont();

		void setColor(uint32 c);

		uint32  getColor();

		void setZ(float z);

		float  getZ();

	private:
		Font::Ptr font;
		float   z;
		uint32   color;
	};

	GRAPHICS_API Canvas* getCanvas();
}

#endif
