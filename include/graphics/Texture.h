//***************************************************************
//  File:    Texture.h
//  Date:    09/30/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef TEXTURE
#define TEXTURE

#include "GraphicsPrerequisites.h"
#include "RenderTarget.h"
#include "Image.h"

namespace hare_graphics 
{
    class GRAPHICS_API Texture : public Object, public RenderTarget
    {
		HARE_DECLARE_ABSTRACT_CLASS(Texture)
    public:
		Texture();

		virtual ~Texture();

        void create(uint32 w, uint32 h, HarePixelFormat format, bool isRenderable = false);
        void createFromFile(const String& filename);     
		
		void setProjectionSize(float w, float h);

		uint32 getWidth(){
            return width;
        }
        uint32 getHeight(){
            return height;
        }

		uint32 getSize() {
			return size;
		}

        const String& getFileName() {
            return fileName;
        }

		bool isRanderAble(){
			return bIsRenderable;
		}

		//Copies an image to the texture.
		virtual void upload(const Image& img, uint32 destX = 0, uint32 destY = 0) = 0;

		//Gets an image from a texture.
		virtual void download(Image& img, const RectN& rc = RectN(0,0,0,0)) = 0;
        virtual void reCreate() = 0;
		virtual void release() = 0;

    protected:
        String fileName;

		uint32 width;
		uint32 height;
        uint32 size;

		HarePixelFormat texPixelFormat;

		float projectionWidth;
		float projectionHeight;

        bool bIsRenderable;
		bool bFromImage;

    };

}
#endif
