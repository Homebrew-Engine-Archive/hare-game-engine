//***************************************************************
//  File:    Material.h
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
#ifndef _MATERIAL_H_
#define _MATERIAL_H_


#include "GraphicsPrerequisites.h"
#include "Texture.h"

namespace hare
{
    /** Store Texture stage params.
    */
	class GRAPHICS_API TextureStage : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(TextureStage)
	public:

        /// Enum identifying color and alpha operation when multiple texture.
		enum ColorAlphaBlendOperation
		{
            /// Params multiplied.
			CABO_Modulate,
            /// Select param one.
			CABO_Select1,
            /// Params add
			CABO_Add,
            /// Use current alpha as linear mixed param
			CABO_BlendTexAlpha,
		};

        /// Enum identifying color and alpha param when multiple texture. 
		enum ColorAlphaBlendArgument
		{
            /// Use current stage texture blend value.
			CBBA_Current,
            /// Use current texture value.
			CABA_Texture,
            /// Use Diffuse Reflectance value.
			CABA_Diffuse,
		};

        /// Enum identifying texture address mode
		enum WrapMode
		{
            /// Repeat address.
			WM_Wrap,
            /// Mirror address.
			WM_Mirror,
            /// Border address color
			WM_Clamp,
            /// Border color by user define.
			WM_Shadow,
		};

        /// Enum identifying filter type
		enum FliterType
		{
            /// point flit
			FT_Point,	    
            /// line  flit
			FT_Line,		
		};

		TextureStage();
		virtual ~TextureStage(){}

		ColorAlphaBlendOperation ColorBlendOP;
		ColorAlphaBlendArgument  ColorBlendArg1;
		ColorAlphaBlendArgument  ColorBlendArg2;

		ColorAlphaBlendOperation AlphaBlendOP;
		ColorAlphaBlendArgument  AlphaBlendArg1;
		ColorAlphaBlendArgument  AlphaBlendArg2;

		uint8 wrapModeU;
		uint8 wrapModeV;

		uint8 fliterType;

		bool operator != (const TextureStage& right);
		bool operator == (const TextureStage& right);

		TextureStage& operator = (const TextureStage& right);
	};


	class TextureMtrl;
	class Shader;

    /** The base class material.
    */ 
	class GRAPHICS_API Material : public Object
	{
		HARE_DECLARE_ABSTRACT_CLASS(Material)
	public:
		Material();
		virtual ~Material();

        /** Operator texture matrix.
        */
		virtual void frameMove() = 0;
		
        /** Get TextureMtrl object pointer.
        */
        virtual TextureMtrl* getTextureMtrl() = 0;
		
        /** Get Shader material object pointer.
        */
        virtual Shader* getShader() = 0;

	};


	/** The base class standard material. 
        Material derived class must implement StandardMtrl.
    */
    class GRAPHICS_API StandardMtrl : public Material
	{
		HARE_DECLARE_ABSTRACT_CLASS(StandardMtrl)
	public:
		StandardMtrl();
		virtual ~StandardMtrl();

		virtual TextureMtrl* getTextureMtrl()
		{
			return NULL;
		}
		virtual Shader* getShader()
		{
			return NULL;
		}

	};


    /** The final material node.
        TextureMtrl stores texture.
    */
	class GRAPHICS_API TextureMtrl : public StandardMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(TextureMtrl)
	public:
		TextureMtrl();
		virtual ~TextureMtrl();

		virtual void frameMove();
		virtual TextureMtrl* getTextureMtrl()
		{
			return this;
		}

        /** Set texture.
        */
		void setTexture(Texture* tex)
		{
			texture = tex;
			fileName = texture->getFileName();
		}

        /** Get texture.
        */
		Texture* getTexture()
		{
			return texture;
		}

        /** Set TextureStage.
        */
		void setTextureStage(const TextureStage& ts)
		{
			*textureStage = ts;
		}

        /** get TextureStage.
        */
		const TextureStage& getTextureStage()
		{
			return *textureStage;
		}

        /** create TextureMtrl from image file
            exp: TextureMtrl::Ptr texMtrl = new TextureMtrl;
                 texMtrl->fileName = "xxx.png";
                 texMtrl->postLoaded();
        */
		virtual void postLoaded();

		virtual void postEdited(Attribute *attr);

	protected:
		Texture::Ptr texture;
		TextureStage::Ptr textureStage;

	public:
        /// texture matrix
		Matrix4      texMat;
        /// image file name 
		String       fileName;

	};

	/** Node material store sub material.
    */
	class GRAPHICS_API WrapperMtrl : public StandardMtrl
	{
		HARE_DECLARE_ABSTRACT_CLASS(WrapperMtrl)
	public:
		WrapperMtrl();
		virtual ~WrapperMtrl();

        /** Get sub material.
        */
		virtual StandardMtrl* getSubMtrl()
		{
			return mtrl;
		}

        /** Set sub material.
        */
		virtual void setSubMtrl(StandardMtrl* m)
		{
			mtrl = m;
		}

		virtual TextureMtrl* getTextureMtrl()
		{
			return mtrl != NULL ? mtrl->getTextureMtrl() : NULL;
		}

	protected:
		StandardMtrl::Ptr mtrl;
	};


    /** Translate material
    */
	class GRAPHICS_API PannerMod : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(PannerMod)
	public:
		PannerMod();
		virtual ~PannerMod();

		virtual void frameMove();
	public:
        /// Translation offset
        PointF  offset;
        /// Translation direction
		PointF  panDirection;
        /// Translation rate
		float   panRate;

		PointF  oscillationPhase;
		PointF  oscillationAmplitude;
		PointF  oscillationRate;
	protected:
		float   startTime;

	};

    /** Scale material.
    */
	class GRAPHICS_API ScalerMod : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(ScalerMod)
	public:
		ScalerMod();
		virtual ~ScalerMod();

		virtual void frameMove();
	public:
        /// scale size.
		PointF	scale;
        /// scale center.
		PointF	center;

		PointF	oscillationPhase;
		PointF	oscillationAmplitude;
		PointF	oscillationRate;
	protected:
		float   startTime;

	};

    /** rotate material
    */
	class GRAPHICS_API RotatorMod : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(RotatorMod)
	public:
		RotatorMod();
		virtual ~RotatorMod();

		virtual void frameMove();
	public:
        /// rotate center
		PointF      center;
        /// rotate offset
		float		rotation;
        /// rotate speed
		float		speed;

		float		oscillationPhase;
		float		oscillationAmplitude;
		float		oscillationRate;

	protected:
		float		startTime;

	};


}

#endif
