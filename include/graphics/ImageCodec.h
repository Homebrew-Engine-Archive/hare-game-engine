#ifndef IMAGECODEC
#define IMAGECODEC

#include "GraphicsPrerequisites.h"
#include "DataHolder.h"
#include "HareGraphicsType.h"

namespace hare_graphics
{
	class GRAPHICS_API ImageCodec
	{
	protected:
		typedef std::map<String, ImageCodec*> CodecMap;

		static CodecMap codecMap;

	public:
		static void registerCodec(ImageCodec* imageCodec);
		static void unregisterCodec(ImageCodec* imageCodec);
		static StringVector getExtensions();
		static ImageCodec* getCodec(const String& extension);

	public:
		ImageCodec(const String& type, u32 devILTypeID);
		virtual ~ImageCodec(void);

		virtual const String& getType() = 0;
		virtual bool encode(const DataHolder& input, DataHolder& output, const ImageInfo& info) = 0;
		virtual bool decode(const DataHolder& input, DataHolder& output, ImageInfo& info) = 0;
		virtual bool codeToFile(const DataHolder& input, const String& fileName, const ImageInfo& info) = 0; 
	
	protected:
		String codecType;
		u32    typeID;
	};
}

#endif