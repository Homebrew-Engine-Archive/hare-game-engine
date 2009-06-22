//***************************************************************
//  File:    Image.h
//  Date:    11/02/2008
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "GraphicsPrerequisites.h"
#include "HareGraphicsType.h"


namespace hare
{
    /** Image class stores image data 
    */
	class GRAPHICS_API Image
	{
	public:
		Image();
		Image(const Image& rhs);
		virtual ~Image();

        /** Create empty image  the HarePixelFormat should be HPF_A8R8G8B8
		*/
        void create(uint32 width, uint32 height, HarePixelFormat format);
		
        /** Destroy image
        */
        void destroy();

        /** Load image data from image file
		*/
        bool loadFromFile(const String& fileName);

        /** Load image data from memory
		*/
        bool loadFromMemory(const DataHolder& input, const String& type);
		
        /** Save image data to file
        */
        bool saveToFile(const String& fileName);

        /** Get image width
        */
		uint32 getWidth() const;

        /** Get image height
        */
		uint32 getHeight() const;
		
        /** Get image pixel format
        */
        HarePixelFormat getPixelFormat() const;

        /** Get image data
		*/
        void* getImageData() const;
		
        /** Get image size
        */
        uint32  getImageSize() const;
		
        /** Get bytes per row
        */
        uint32  getRowStride() const;

        /** scale image 
		*/
        bool  scale(int newWidth, int newHeight);

		const Image& operator = (const Image& rimg);
	
    protected:
		String		fileName;
		ImageInfo	imageInfo;
		DataHolder	imageData;
	};
}

#endif
