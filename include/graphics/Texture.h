//***************************************************************
//  File:    Texture.h
//  Date:    09/30/2008
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "GraphicsPrerequisites.h"
#include "RenderTarget.h"
#include "Image.h"

namespace hare
{
    /** The base class Texture.
    */
    class GRAPHICS_API Texture : public Object, public RenderTarget
    {
		HARE_DECLARE_ABSTRACT_CLASS(Texture)
    public:
		Texture();

		virtual ~Texture();

        /** Create empty texture. 
        */
        void create(uint32 w, uint32 h, HarePixelFormat format, bool isRenderable = false);
        
        /** Create texture from file.
        */
        void createFromFile(const String& filename);

        /** Set projection size for dynamic texture.
            default size is texture size.
        */
		void setProjectionSize(float w, float h);

        /** Get texture width.
        */
		uint32 getWidth(){
            return width;
        }

        /** Get texture height.
        */
        uint32 getHeight(){
            return height;
        }

        /** Get file name.
        */
        const String& getFileName() {
            return fileName;
        }

        /** Is dynamic texture.
        */
		bool isRenderable(){
			return bIsRenderable;
		}

		/** Copies an image to the texture.
        */
		virtual void upload(const Image& img, uint32 destX = 0, uint32 destY = 0) = 0;

		/** Gets an image from a texture.
        */
		virtual void download(Image& img, const RectN& rc = RectN(0,0,0,0)) = 0;
        
        virtual void recreate() = 0;
		virtual void release() = 0;

    protected:
        String fileName;

		uint32 width;
		uint32 height;

		HarePixelFormat texPixelFormat;

		float projectionWidth;
		float projectionHeight;

        bool bIsRenderable;
		bool bFromImage;

    };

}
#endif
