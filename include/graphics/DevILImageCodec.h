#ifndef DEVIDIMAGECODEC
#define DEVIDIMAGECODEC

#include "GraphicsPrerequisites.h"
#include "ImageCodec.h"

namespace hare_graphics
{
	class GRAPHICS_API DevILImageCodec : public ImageCodec
	{
	public:
		DevILImageCodec(const String& type, u32 devILTypeID); //‘⁄devil¿Ô ILenum «ID
		virtual ~DevILImageCodec(void);

		virtual const String& getType();
		virtual bool codeToFile(const DataHolder& input, const String& fileName, const ImageInfo& info);
		virtual bool encode(const DataHolder& input, DataHolder& output, const ImageInfo& info);
		virtual bool decode(const DataHolder& input, DataHolder& output, ImageInfo& info);

	public:
		static void startUp();
		static void shutDown();

	private:
		static void initializion();
		static bool isInitialised;

	};
}

#endif
