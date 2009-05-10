//***************************************************************
//  File:    FreeImageCodec.h
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _FREEIMAGECODEC_H_
#define _FREEIMAGECODEC_H_

#include "ImageCodec.h"

namespace hare
{
	class FreeImageCodec : public ImageCodec
	{
	public:
		FreeImageCodec(const String& type, uint32 freeImgTypeID);//在freeimage里 FREE_IMAGE_FORMAT 作为typeID
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
