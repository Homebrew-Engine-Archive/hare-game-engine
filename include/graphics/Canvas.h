#ifndef _CANVAS_H_
#define _CANVAS_H_

namespace hare
{
	class Material;
	class Font;

	class GRAPHICS_API Canvas : public Singleton<Canvas>
	{
		HARE_DECLARE_SINGLETON(Canvas)
	public:
		Canvas();

		void drawLine(int x1, int y1, int x2, int y2);

		void drawRect(int l, int t, int r, int b);

		void drawText(int x, int y, const String& text);

		void drawImage(int x, int y, Material* mtrl, const RectF& uvRect = RectF(0,0,1,1));

		void drawImage(const RectF& rect, Material* mtrl, const RectF& uvRect = RectF(0,0,1,1));

		void drawImage(const RectF& rect, Material* mtrl, float rot, const RectF& uvRect = RectF(0,0,1,1), const PointF& center = PointF(0.5f, 0.5f));

		void setFont(Font* f);

		void setColor(uint32 c);

		uint32  getColor();

		void setZ(float z);

		float  getZ();

	private:
		Font* font;
		float   z;
		uint32   color;
	};

	GRAPHICS_API Canvas* getCanvas();
}

#endif
