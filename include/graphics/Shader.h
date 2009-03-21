#ifndef SHADER
#define SHADER


#include "GraphicsPrerequisites.h"
//着色器
#include "Material.h"

namespace hare_graphics
{
	class GRAPHICS_API ShaderParams
	{
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
		SceneBlendOperation   SceneBlendOP;
		SceneBlendArgument    SceneBlendSrcArg;
		SceneBlendArgument    SceneBlendDesArg;

		bool AlphaTestEnable;
		u8   AlphaRef;

		bool operator != (const ShaderParams& right);
		bool operator == (const ShaderParams& right);
	};

	//着色器基类

	class GRAPHICS_API Shader : public Object
	{
		HARE_DECLARE_ABSTRACT_CLASS(Shader)
	public:
		Shader();
		virtual ~Shader();

		virtual void setMaterial(Material* m) = 0;
		virtual Material* getMaterial() = 0;
		virtual TextureMtrl* getTextureMtrl() = 0;

		void setShaderParams(const ShaderParams& s)
		{
			shaderParams = s;
		}

		const ShaderParams& getShaderParams()
		{
			return shaderParams;
		}


	protected:
		ShaderParams shaderParams;
	};

	class GRAPHICS_API SimpleShader : public Shader
	{
		HARE_DECLARE_DYNAMIC_CLASS(SimpleShader)
	public:
		SimpleShader();
		virtual ~SimpleShader();

		virtual void setMaterial(Material* m)
		{
			mtrl = m;
		}

		virtual Material* getMaterial()
		{
			return mtrl;
		}

		virtual TextureMtrl* getTextureMtrl()
		{
			return mtrl != NULL ? mtrl->getTextureMtrl() : NULL;
		}

	protected:
		Material::Ptr mtrl;
	};


	class GRAPHICS_API ParticleShader : public Shader
	{
		HARE_DECLARE_DYNAMIC_CLASS(ParticleShader)
	public:
		ParticleShader();

		virtual ~ParticleShader();

		virtual void setMaterial(Material* m)
		{
			mtrl = m;
		}

		virtual Material* getMaterial()
		{
			return mtrl;
		}

		virtual TextureMtrl* getTextureMtrl()
		{
			if (!mtrl)
				return NULL;

			TextureMtrl* texMtrl = mtrl->getTextureMtrl();
			if (texMtrl){
				TextureStage texStage;
				texStage.AlphaBlendOP   = TextureStage::CABO_Select1;
				texStage.AlphaBlendArg1 = TextureStage::CABA_Texture;

				texMtrl->setTextureStage(texStage);	
			}

			return texMtrl;
		}

	protected:
		Material::Ptr mtrl;

	};
}

#endif