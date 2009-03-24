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

        void createRenderToTex(u32 w, u32 h, HarePixelFormat format, bool isRenderable = false);
        void createFromFile(const String& filename);     
		
		void setProjectionSize(f32 w, f32 h);

		u32 getWidth(){
            return width;
        }
        u32 getHeight(){
            return height;
        }

		u32 getSize() {
			return size;
		}

        const String& getFileName() {
            return fileName;
        }

		bool isRanderAble(){
			return bIsRenderable;
		}

		//Copies an image to the texture.
		virtual void upload(const Image& img, u32 destX = 0, u32 destY = 0) = 0;

		//Gets an image from a texture.
		virtual void download(Image& img, const RectN *rc = 0) = 0;
        virtual void reCreate() = 0;
		virtual void release() = 0;

    protected:
        String fileName;

		u32 width;
		u32 height;
        u32 size;

		HarePixelFormat texPixelFormat;

		f32 projectionWidth;
		f32 projectionHeight;

        bool bIsRenderable;
		bool bFromImage;

    };

}
#endif
