//***************************************************************
//  File:    Shader.h
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
#ifndef _SHADER_H_
#define _SHADER_H_

#include "GraphicsPrerequisites.h"
#include "Material.h"

namespace hare
{
    /** Shadow param
    */
	class GRAPHICS_API ShaderParams : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(ShaderParams)
	public:
        /// Enum identifying blend operator.
		enum SceneBlendOperation{
            /// The result is the destination added to the source. Result = Source + Destination
			SBO_Add,
            /// The result is the source subtracted from the destination. Result = Destination - Source
			SBO_Revsubtract,
            /// The result is the minimum of the source and destination. Result = MIN(Source, Destination)
			SBO_Min,
            /// The result is the maximum of the source and destination. Result = MAX(Source, Destination)
			SBO_Max,
		};

        /// Enum identifying blend mode
		enum SceneBlendArgument{
            /// Blend factor is (As, As, As, As).
			SBA_Srcalpha,
			/// Blend factor is ( 1 - As, 1 - As, 1 - As, 1 - As).
            SBA_Invsrcalpha,
			/// Blend factor is (1, 1, 1, 1).
            SBA_One,
			/// Blend factor is (0, 0, 0, 0).
            SBA_Zero,
		};

		ShaderParams();
		~ShaderParams(){}

        /// enable alpha blend
		bool AlphaBlendEnable;

        /// blend operator
		uint8 SceneBlendOP;
        /// source blend model
		uint8 SceneBlendSrcArg;
		/// destination blend model
        uint8 SceneBlendDesArg;

        /// enable alpha test
		bool  AlphaTestEnable;
        /// the threshold value for alpha testing
		uint8 AlphaRef;

        /// enable z
		bool  bUseZ;

		bool operator != (const ShaderParams& right);
		bool operator == (const ShaderParams& right);

		ShaderParams& operator = (const ShaderParams& right);
	};

    /** The shadow base class
    */
	class GRAPHICS_API Shader : public Material
	{
		HARE_DECLARE_ABSTRACT_CLASS(Shader)
	public:
		Shader();
		virtual ~Shader();

        /** Set material
        */
		virtual void setMaterial(StandardMtrl* m) = 0;
		
        /** Get material
        */
        virtual StandardMtrl* getMaterial() = 0;

		virtual Shader* getShader()
		{
			return this;
		}

        /** Set shadow params
        */
		void setShaderParams(const ShaderParams& s)
		{
			*shaderParams = s;
		}

        /** Get shadow params
        */
		const ShaderParams& getShaderParams()
		{
			return *shaderParams;
		}


	protected:
		ShaderParams::Ptr shaderParams;
	};


    /** implement Shader class.
        ShaderParams use default params
        alpha test enable alpha blend disable
    */
	class GRAPHICS_API SimpleShader : public Shader
	{
		HARE_DECLARE_DYNAMIC_CLASS(SimpleShader)
	public:
		SimpleShader();
		virtual ~SimpleShader();

		virtual void frameMove()
		{
			if (mtrl)
				mtrl->frameMove();
		}

		virtual void setMaterial(StandardMtrl* m)
		{
			mtrl = m;
		}

		virtual StandardMtrl* getMaterial()
		{
			return mtrl;
		}

		virtual TextureMtrl* getTextureMtrl()
		{
			return mtrl != NULL ? mtrl->getTextureMtrl() : NULL;
		}

	protected:
		StandardMtrl::Ptr mtrl;
	};


    /** Alpha blend enable alpha test disable
    */
	class GRAPHICS_API ParticleShader : public Shader
	{
		HARE_DECLARE_DYNAMIC_CLASS(ParticleShader)
	public:
		ParticleShader();

		virtual ~ParticleShader();

		virtual void frameMove()
		{
			if (mtrl)
				mtrl->frameMove();
		}

		virtual void setMaterial(StandardMtrl* m)
		{
			mtrl = m;
		}

		virtual StandardMtrl* getMaterial()
		{
			return mtrl;
		}

		virtual TextureMtrl* getTextureMtrl()
		{
			return mtrl != NULL ? mtrl->getTextureMtrl() : NULL;
		}

	protected:
		StandardMtrl::Ptr mtrl;

	};
}

#endif
