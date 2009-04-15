#include "PCH.h"
#include "DevILImageCodec.h"
#include "GraphicsUtil.h"
#include "IL/il.h"

namespace hare
{
    static void toDevILFormat(const HarePixelFormat& hpf, int &imageFormat, int& bytesPerPixel)
    {
        switch (hpf)
        {
        case HPF_A8R8G8B8:
            {
                imageFormat = IL_BGRA;
                bytesPerPixel = 4;
                break;
            }
		case HPF_A8B8G8R8:
			{
				imageFormat = IL_RGBA;
				bytesPerPixel = 4;
				break;
			}
        default:assert(false);
        }
    }

    static void toHarePixelFormat(HarePixelFormat& hpf, int imageFormat, int bytesPerPixel)
    {
        switch (bytesPerPixel)
        {
        case 4:
            switch (imageFormat)
            {
            case IL_BGRA:
                hpf = HPF_A8R8G8B8;
                break;
			case IL_RGBA:
				hpf = HPF_A8B8G8R8;
				break;
			default:assert(false);
            }
			break;
        default:assert(false);
        }
    }

	struct RegisterDataBase
	{
		char* type;
		uint32  devILTypeID;
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
			ilEnable(IL_FILE_OVERWRITE);
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


	DevILImageCodec::DevILImageCodec(const String& type, uint32 devILTypeID)
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
		toDevILFormat(info.format, imageFormat, bytesPerPixel);

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

		uint32 imageSize = ilGetInteger( IL_IMAGE_WIDTH ) * ilGetInteger( IL_IMAGE_HEIGHT ) * ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );
		output.allocate( imageSize );

		memcpy(output.getData(), data, imageSize); //ilGetData(), ImageSize);
		toHarePixelFormat(info.format, imagformat, bytesPerPixel );

		//for windows Í¼Æ¬·­×ª
		//if (ilImg->Data != data)
		//	ifree(data); // image flipped, free temp data.

		ilDeleteImages( 1, &imageName );

		if (info.format == HPF_UnKnow)
			return false;

		return true;
	}
}
