#include "PCH.h"
#include "DevILImageCodec.h"
#include "GraphicsUtil.h"
#include "IL/il.h"
//#include "IL/devil_internal_exports.h"



namespace hare_graphics
{
	struct RegisterDataBase
	{
		char* type;
		u32  devILTypeID;
	};

	RegisterDataBase registerDataBase[] =
	{
		{"bmp", IL_BMP},
		{"png", IL_PNG},
		{"tga", IL_TGA}
	};

	bool DevILImageCodec::isInitialised = false;
	void DevILImageCodec::initializion()
	{
		if (!isInitialised){
			ilInit();
			ilEnable( IL_FILE_OVERWRITE );
			isInitialised = true;
		}
	}

	void DevILImageCodec::startUp()
	{
		for (int count = 0; count < sizeof(registerDataBase)/sizeof(registerDataBase[0]); ++count){
			ImageCodec* tmp = new DevILImageCodec(registerDataBase[count].type, registerDataBase[count].devILTypeID);
			codecMap[tmp->getType()] = tmp;
		}
	}

	void DevILImageCodec::shutDown()
	{
		CodecMap::iterator it = codecMap.begin();
		for (;it != codecMap.end(); ++it){
			delete it->second;
		}
	}


	DevILImageCodec::DevILImageCodec(const String& type, u32 devILTypeID)
		:ImageCodec(type, devILTypeID)
	{
		initializion();
	}

	DevILImageCodec::~DevILImageCodec()
	{
	}

	const String& DevILImageCodec::getType()
	{
		return codecType;
	}

	bool DevILImageCodec::codeToFile(const DataHolder& input, const String& fileName, const ImageInfo& info)
	{
		ILuint	imageID;

		// ¨¦¨²3¨¦ image
		ilGenImages(1, &imageID);

		// Bind image
		ilBindImage(imageID);

		int imageFormat, bytesPerPixel;
		GraphicsUtil::HarePixelFormat2DevILFormat(info.format, imageFormat, bytesPerPixel);

		// ¨¬?3? image ¦Ì?¨ºy?Y
		ILboolean ret = ilTexImage(info.width, info.height, 1, bytesPerPixel, imageFormat, IL_UNSIGNED_BYTE, input.getData());
		if (ret == IL_ILLEGAL_OPERATION || ret == IL_INVALID_PARAM || ret == IL_OUT_OF_MEMORY)
			return false;

		ret = ilSave(typeID, const_cast<char*>(fileName.c_str()));
		if (ret == IL_COULD_NOT_OPEN_FILE || ret ==  IL_INVALID_ENUM)
			return false;

		ilDeleteImages(1, &imageID);

		return true;
	}

	bool DevILImageCodec::encode(const DataHolder& input, DataHolder& output, const ImageInfo& info)
	{
		return true;
	}

	bool DevILImageCodec::decode(const DataHolder& input, DataHolder& output, ImageInfo& info)
	{

		ILuint imageName;
		ILint imagformat, bytesPerPixel;

		ilGenImages( 1, &imageName );
		ilBindImage( imageName );


		if (!ilLoadL( typeID, input.getData(), static_cast< ILuint >(input.getSize()) )) {
			return false;
		}

		imagformat = ilGetInteger( IL_IMAGE_FORMAT );
		bytesPerPixel = ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );
		info.width = ilGetInteger( IL_IMAGE_WIDTH );
		info.height = ilGetInteger( IL_IMAGE_HEIGHT );

		//for windows Í¼Æ¬·­×ª
		//ILimage *ilImg = ilGetCurImage();
		//ILubyte	*data = ilImg->Data; // or ilGetData()

		ILubyte	*data = ilGetData();

		//if ((ILenum)ilGetInteger(IL_IMAGE_ORIGIN) == IL_ORIGIN_UPPER_LEFT) {
		//	data = iGetFlipped(ilImg);
		//}

		u32 imageSize = ilGetInteger( IL_IMAGE_WIDTH ) * ilGetInteger( IL_IMAGE_HEIGHT ) * ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );
		output.allocate( imageSize );

		// converts pixel format.
		struct _ARGB { u8 a, r, g, b; };
		struct _RGBA { u8 r, g, b, a; };
		struct _BGRA { u8 b, g, r, a; };	// NOTE: match with PF_A8R8G8B8
		struct _BGR { u8 b, g, r; };		// NOTE: match with PF_R8G8B8
		struct _RGB { u8 r, g, b; };
		int pixels = info.width * info.height;
		if (imagformat == IL_RGBA && bytesPerPixel == 4) {
			// RGBA to BGRA
			_RGBA *from = (_RGBA*)data;//ilGetData();
			_RGBA temp;
			_BGRA *to = (_BGRA*)output.getData();
			for (int i = 0; i < pixels; i ++) {
				temp = *from;
				to->a = temp.a;
				to->r = temp.r;
				to->g = temp.g;
				to->b = temp.b;

				from ++;
				to ++;
			}
			info.format = HPF_A8R8G8B8;
		}
		else {
			memcpy(output.getData(), data, imageSize); //ilGetData(), ImageSize);
			GraphicsUtil::DevILFormat2HarePixelFormat(info.format, imagformat, bytesPerPixel );
		}

		//for windows Í¼Æ¬·­×ª
		//if (ilImg->Data != data)
		//	ifree(data); // image flipped, free temp data.

		ilDeleteImages( 1, &imageName );

		if (info.format == HPF_UnKnow)
			return false;

		return true;
	}

}
