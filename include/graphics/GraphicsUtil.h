#ifndef GRAPHICSUTIL
#define GRAPHICSUTIL

#include "GraphicsPrerequisites.h"
#include "HareGraphicsType.h"

namespace hare_graphics
{

	class GRAPHICS_API GraphicsUtil
	{
	public:
		static void HarePixelFormat2FreeImageFormat(const HarePixelFormat& hpf, int &ImageFormat, int &BytesPerPixel);
		static void FreeImageFormat2HarePixelFormat(HarePixelFormat& hpf, int ImageFormat, int BytesPerPixel);
		static void HarePixelFormat2DevILFormat(const HarePixelFormat& hpf, int &ImageFormat, int &BytesPerPixel);
		static void DevILFormat2HarePixelFormat(HarePixelFormat& hpf, int ImageFormat, int BytesPerPixel);
		static u32  GetPixelFormatBytes(HarePixelFormat hpf);
	};

}
#endif
