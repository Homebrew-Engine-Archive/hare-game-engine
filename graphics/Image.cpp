//***************************************************************
//  File:    Image.cpp
//  Data:    11/02/2008
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#include "PCH.h"
#include "Image.h"
#include "2PassScale.h"
#include "GraphicsUtil.h"
#include "ImageCodec.h"

namespace hare_graphics
{
	Image::Image(void)
	{

	}

	Image::Image(const Image& rhs)
	{
		*this = rhs;
	}

	Image::~Image(void)
	{
		
	}

	void Image::create(u32 width, u32 height, HarePixelFormat format)
	{
		destory();
		imageInfo.width = width;
		imageInfo.height= height;
		imageInfo.format= format;
		u32 size = width * height * GraphicsUtil::GetPixelFormatBytes(format);
		imageData.allocate(size);
	}

	void Image::destory()
	{
		imageData.clear();

		imageInfo.width = 0;
		imageInfo.height= 0;
		imageInfo.format= HPF_UnKnow;
	}

	bool Image::loadFromFile(const String& fileName)
	{
		String baseFileName, fileExt;
		StringUtil::splitFilename(fileName, baseFileName, fileExt);
		DataHolder input;
		/*文件系统打开文件将数据读进input
		*/
		FileSystem* fs = getFileSystem();
		FileHandle fh = fs->openFile(fileName, FM_Read);

		if (!fh)
			return false;

		int size = fs->size(fh);
		input.allocate(size);
		fs->readFile(fh, (char*)input.getData(), size, 1);
		fs->closeFile(fh);

		return loadFromMemery(input, fileExt);
	}

	bool Image::loadFromMemery(const DataHolder& input, const String& type)
	{
		ImageCodec* imageCodec = ImageCodec::getCodec(type);

		DataHolder output;
		ImageInfo  info;

		if (!imageCodec->decode(input, output, info))
			return false;

		imageData.allocate(output.getSize());
		memcpy(imageData.getData(), output.getData(), output.getSize());
		imageInfo = info;

		return true;
	}

	bool Image::saveToFile(const String& fileName)
	{
		String baseFileName, fileExt;
		StringUtil::splitFilename(fileName, baseFileName, fileExt);
		ImageCodec* imageCodec = ImageCodec::getCodec(fileExt);

		return imageCodec->codeToFile(imageData, fileName, imageInfo);
	}

	u32 Image::getWidth() const
	{
		return imageInfo.width;
	}

	u32 Image::getHeight() const
	{
		return imageInfo.height;
	}

	HarePixelFormat Image::getPixelFormat() const
	{
		return imageInfo.format;
	}

	void* Image::getImageData() const
	{
		return imageData.getData();
	}

	u32  Image::getImageSize() const
	{
		return imageData.getSize();
	}

	u32  Image::getRowStride() const
	{
		return GraphicsUtil::GetPixelFormatBytes(getPixelFormat()) * imageInfo.width;
	}

	bool  Image::scale(int newWidth, int newHeight)
	{
		if (!imageData.getData() || newWidth < 0 || newHeight < 0)
			return false;
		
		Image newImage;
		newImage.create(newWidth, newHeight, imageInfo.format);

		switch (imageInfo.format) {

		case HPF_A8R8G8B8: {
			typedef HarePixelType<HPF_A8R8G8B8> MyPixelType;
			C2PassScale<CBoxFilter, MyPixelType> ps;

			ps.Scale((MyPixelType*)imageData.getData(), imageInfo.width, imageInfo.height, (MyPixelType*)newImage.getImageData(), newImage.getWidth(), newImage.getHeight());
			}
			break;
		default:
			return false;
		}

		*this = newImage;
		return true;
	}

	const Image& Image::operator = (const Image& rimg)
	{
		fileName = rimg.fileName;
		imageInfo = rimg.imageInfo;
		imageData.allocate(rimg.getImageSize());
		memcpy(imageData.getData(), rimg.getImageData(), rimg.getImageSize());
		return *this;
	}
}
