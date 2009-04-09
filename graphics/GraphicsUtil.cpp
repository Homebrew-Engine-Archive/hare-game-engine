#include "PCH.h"
#include "GraphicsUtil.h"

namespace hare_graphics
{
	//void GraphicsUtil::HarePixelFormat2FreeImageFormat(const HarePixelFormat& hpf, int &ImageFormat, int &BytesPerPixel)
	//{
	//	//switch(hpf)
	//	//{
	//	//case HPF_A8R8G8B8:
	//	//	{
	//	//	ImageFormat = FIT_BITMAP;
	//	//	BytesPerPixel = 4;
	//	//	}
	//	//default:assert(false);
	//	//}
	//}

	//void GraphicsUtil::FreeImageFormat2HarePixelFormat(HarePixelFormat& hpf, int ImageFormat, int BytesPerPixel)
	//{
	//	//switch(BytesPerPixel)
	//	//{
	//	//case 4:
	//	//	switch(ImageFormat)
	//	//	{
	//	//	case FIT_BITMAP:
	//	//		hpf = HPF_A8R8G8B8;
	//	//		break;
	//	//	}
	//	//default:assert(false);
	//	//}
	//}

	u32 GraphicsUtil::getPixelFormatBytes(HarePixelFormat hpf)
	{
		switch(hpf)
		{
		case HPF_A8R8G8B8: 
            return 4;
		case HPF_A8B8G8R8:
			return 4;
		case HPF_BYTE_LA: 
            return 2;
		default:
            assert(false);
		}
		return 0;
	}
}
