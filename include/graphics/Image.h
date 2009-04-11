#ifndef IMAGE
#define IMAGE

#include "GraphicsPrerequisites.h"
#include "HareGraphicsType.h"
#include "DataHolder.h"

namespace hare_graphics
{

	class GRAPHICS_API Image
	{
	public:
		Image();
		Image(const Image& rhs);
		virtual ~Image();

		void create(uint32 width, uint32 height, HarePixelFormat format);
		void destory();

		bool loadFromFile(const String& fileName);
		bool loadFromMemery(const DataHolder& input, const String& type);
		bool saveToFile(const String& fileName);

		uint32 getWidth() const;
		uint32 getHeight() const;
		HarePixelFormat getPixelFormat() const;

		void* getImageData() const;
		uint32  getImageSize() const;
		uint32  getRowStride() const;//һ�е�size

		bool  scale(int newWidth, int newHeight);

		const Image& operator = (const Image& rimg);
	protected:
		String		fileName;
		ImageInfo	imageInfo;
		DataHolder	imageData;
	};
}

#endif
