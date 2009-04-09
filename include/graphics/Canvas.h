#ifndef _CANVAS_H_
#define _CANVAS_H_

namespace hare_graphics
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

		void drawImage(const RectF& rect, Material* mtrl, f32 rot, const RectF& uvRect = RectF(0,0,1,1), const PointF& center = PointF(0.5f, 0.5f));

		void setFont(Font* f);

		void setColor(u32 c);

		u32  getColor();

		void setZ(f32 z);

		f32  getZ();

	private:
		Font* font;
		f32   z;
		u32   color;
	};

	GRAPHICS_API Canvas* getCanvas();
}

#endif
