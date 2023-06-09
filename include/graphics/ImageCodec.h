//***************************************************************
//  File:    ImageCodec.h
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
#ifndef _IMAGECODEC_H_
#define _IMAGECODEC_H_

#include "GraphicsPrerequisites.h"

#include "HareGraphicsType.h"

namespace hare
{
    /** ImageCodec class is used for encoding and decoding image file
	*/
    class GRAPHICS_API ImageCodec
	{
	protected:
		typedef std::map<String, ImageCodec*> CodecMap;

		static CodecMap codecMap;

	public:
		static void registerCodec(ImageCodec* imageCodec);
		static void unregisterCodec(ImageCodec* imageCodec);
		static StringVector getExtensions();

        /** Get ImageCodec object pointer by image format. eg. "png"
		*/
        static ImageCodec* getCodec(const String& extension);

	public:
		ImageCodec(const String& type, uint32 devILTypeID);
		virtual ~ImageCodec(void);

        /** Get encode and decode image type.
		*/
        virtual const String& getType() = 0;
        /** Encode image data
		*/
        virtual bool encode(const DataHolder& input, DataHolder& output, const ImageInfo& info) = 0;
		/** Decode image data
        */
        virtual bool decode(const DataHolder& input, DataHolder& output, ImageInfo& info) = 0;
		/** Encode Image data save to file
		*/
        virtual bool codeToFile(const DataHolder& input, const String& fileName, const ImageInfo& info) = 0;

	protected:
		String codecType;
		uint32    typeID;
	};
}

#endif
