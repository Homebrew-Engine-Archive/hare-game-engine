#ifndef FREEIMAGECODEC
#define FREEIMAGECODEC

#include "ImageCodec.h"

namespace hare_graphics
{
	class FreeImageCodec : public ImageCodec
	{
	public:
		FreeImageCodec(const String& type, u32 freeImgTypeID);//��freeimage�� FREE_IMAGE_FORMAT ��ΪtypeID
		virtual ~FreeImageCodec();

		virtual const String& getType();
		virtual bool codeToFile(const DataHolder& input, const String& fileName, const ImageInfo& info);
		virtual bool encode(const DataHolder& input, DataHolder& output, const ImageInfo& info);
		virtual bool decode(const DataHolder& input, DataHolder& output, ImageInfo& info);

	public:
		static void startUp();
		static void shutDown();
	};
}

#endif
