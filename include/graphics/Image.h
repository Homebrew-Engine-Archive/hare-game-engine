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

		void create(u32 width, u32 height, HarePixelFormat format);
		void destory();

		bool loadFormFile(const String& fileName);
		bool loadFromMemery(const DataHolder& input, const String& type);
		bool saveToFile(const String& fileName);

		u32 getWidth() const;
		u32 getHeight() const;
		HarePixelFormat getPixelFormat() const;

		void* getImageData() const;
		u32  getImageSize() const;
		u32  getRowStride() const;//Ò»ÐÐµÄsize

		bool  scale(int newWidth, int newHeight);

		const Image& operator = (const Image& rimg);
	protected:
		String		fileName;
		ImageInfo	imageInfo;
		DataHolder	imageData;
	};
}


#endif


