#ifndef CANVAS
#define CANVAS

namespace hare_graphics
{
	class Shader;

	class GRAPHICS_API Canvas : public Singleton<Canvas>
	{
		HARE_DECLARE_SINGLETON(Canvas)
	public:
		Canvas();
		~Canvas();

		void drawLine(int x1, int y1, int x2, int y2, u32 color, f32 z = 0.f);

		void drawRect(int l, int t, int r, int b, u32 color, f32 z = 0.f);

		void drawText(int x, int y, const String& text);

		void drawImage(int x, int y, Shader* shader, f32 z = 0.f);

		void drawImage(const Rect<int>& rect, Shader* shader, float rot = 0.f, f32 z = 0.f);

	};

	GRAPHICS_API Canvas* getCanvas();
}


#endif