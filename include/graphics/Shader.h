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
//着色器
#include "Material.h"

namespace hare
{
	class GRAPHICS_API ShaderParams : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(ShaderParams)
	public:
		enum SceneBlendOperation{
			SBO_Add,
			SBO_Revsubtract,
			SBO_Min,
			SBO_Max,
		};

		enum SceneBlendArgument{
			SBA_Srcalpha,
			SBA_Invsrcalpha,
			SBA_One,
			SBA_Zero,
		};

		ShaderParams();
		~ShaderParams(){}

		bool AlphaBlendEnable;

		uint8 SceneBlendOP;
		uint8 SceneBlendSrcArg;
		uint8 SceneBlendDesArg;

		bool  AlphaTestEnable;
		uint8 AlphaRef;

		bool  bUseZ;

		bool operator != (const ShaderParams& right);
		bool operator == (const ShaderParams& right);

		ShaderParams& operator = (const ShaderParams& right);
	};

	//着色器基类

	class GRAPHICS_API Shader : public Material
	{
		HARE_DECLARE_ABSTRACT_CLASS(Shader)
	public:
		Shader();
		virtual ~Shader();

		virtual void setMaterial(StandardMtrl* m) = 0;
		virtual StandardMtrl* getMaterial() = 0;

		virtual Shader* getShader()
		{
			return this;
		}

		void setShaderParams(const ShaderParams& s)
		{
			*shaderParams = s;
		}

		const ShaderParams& getShaderParams()
		{
			return *shaderParams;
		}


	protected:
		ShaderParams::Ptr shaderParams;
	};

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
