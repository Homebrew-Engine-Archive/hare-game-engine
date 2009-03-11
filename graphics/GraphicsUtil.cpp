#include "PCH.h"
#include "GraphicsUtil.h"
#include "IL/il.h"


namespace hare_graphics
{
	void GraphicsUtil::HarePixelFormat2FreeImageFormat(const HarePixelFormat& hpf, int &ImageFormat, int &BytesPerPixel)
	{
		//switch(hpf)
		//{
		//case HPF_A8R8G8B8:
		//	{
		//	ImageFormat = FIT_BITMAP;
		//	BytesPerPixel = 4;
		//	}
		//default:assert(false);
		//}
	}

	void GraphicsUtil::FreeImageFormat2HarePixelFormat(HarePixelFormat& hpf, int ImageFormat, int BytesPerPixel)
	{
		//switch(BytesPerPixel)
		//{
		//case 4:
		//	switch(ImageFormat)
		//	{
		//	case FIT_BITMAP:
		//		hpf = HPF_A8R8G8B8;
		//		break;
		//	}
		//default:assert(false);
		//}
	}

	void GraphicsUtil::HarePixelFormat2DevILFormat(const HarePixelFormat& hpf, int &ImageFormat, int &BytesPerPixel)
	{
		switch(hpf)
		{
		case HPF_A8R8G8B8:
			{
				ImageFormat = IL_BGRA;
				BytesPerPixel = 4;
				break;
			}
		default:assert(false);
		}
	}

	void GraphicsUtil::DevILFormat2HarePixelFormat(HarePixelFormat& hpf, int ImageFormat, int BytesPerPixel)
	{
		switch(BytesPerPixel)
		{
		case 4:
			switch(ImageFormat)
			{
			case IL_BGRA:
				hpf = HPF_A8R8G8B8;
				break;
			}
		default:assert(false);
		}
	}

	u32 GraphicsUtil::GetPixelFormatBytes(HarePixelFormat hpf)
	{
		switch(hpf)
		{
		case HPF_A8R8G8B8: return 4;
		default:assert(false);
		}
		return 0;
	}
}











