#ifndef SHADER
#define SHADER

#include "GraphicsPrerequisites.h"
//着色器
#include "Material.h"

namespace hare_graphics
{
	class GRAPHICS_API ShaderParams : public Object
	{
		HARE_DECLARE_ABSTRACT_CLASS(ShaderParams)
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

		bool operator != (const ShaderParams& right);
		bool operator == (const ShaderParams& right);
	};

	//着色器基类

	class GRAPHICS_API Shader : public Material
	{
		HARE_DECLARE_ABSTRACT_CLASS(Shader)
	public:
		Shader();
		virtual ~Shader();

		virtual void frameMove() = 0;
		virtual void setMaterial(StandardMtrl* m) = 0;
		virtual StandardMtrl* getMaterial() = 0;
		virtual TextureMtrl* getTextureMtrl() = 0;

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
			if (!mtrl)
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
			if (!mtrl)
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
