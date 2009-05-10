//***************************************************************
//  File:    DevILImageCodec.h
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
#ifndef _DEVIDIMAGECODEC_H_
#define _DEVIDIMAGECODEC_H_

#include "GraphicsPrerequisites.h"
#include "ImageCodec.h"

namespace hare
{
	class GRAPHICS_API DevILImageCodec : public ImageCodec
	{
	public:
		DevILImageCodec(const String& type, uint32 devILTypeID); //‘⁄devil¿Ô ILenum «ID
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
