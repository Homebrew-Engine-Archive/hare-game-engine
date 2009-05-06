#ifndef _MATERIAL_H_
#define _MATERIAL_H_

//材质
#include "GraphicsPrerequisites.h"
#include "Texture.h"

namespace hare
{
	class GRAPHICS_API TextureStage : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(TextureStage)
	public:
		enum ColorAlphaBlendOperation
		{
			CABO_Modulate,
			CABO_Select1,
			CABO_Add,
			CABO_BlendTexAlpha,
		};

		enum ColorAlphaBlendArgument
		{
			CBBA_Current,
			CABA_Texture,
			CABA_Diffuse,
		};

		enum WrapMode
		{
			WM_Wrap,
			WM_Mirror,
			WM_Clamp,
			WM_Shadow,
		};

		enum FliterType
		{
			FT_Point,	    //没有minimap
			FT_Line,		//
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

	//材质基类
	class TextureMtrl;
	class Shader;

	class GRAPHICS_API Material : public Object
	{
		HARE_DECLARE_ABSTRACT_CLASS(Material)
	public:
		Material();
		virtual ~Material();

		virtual void frameMove() = 0;
		virtual TextureMtrl* getTextureMtrl() = 0;
		virtual Shader* getShader() = 0;

	};


	//节点材质
	//创建这一层抽象类使保存材质文件明确
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


	//纹理材质
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

		void setTexture(Texture* tex)
		{
			texture = tex;
			fileName = texture->getFileName();
		}
		Texture* getTexture()
		{
			return texture;
		}

		void setTextureStage(const TextureStage& ts)
		{
			*textureStage = ts;
		}

		const TextureStage& getTextureStage()
		{
			return *textureStage;
		}

		virtual void postLoaded();

		virtual void postEdited(Attribute *attr);

	protected:
		Texture::Ptr texture;
		TextureStage::Ptr textureStage;

	public:
		Matrix4      texMat;
		String       fileName;

	};

	//节点包裹材质基类
	class GRAPHICS_API WrapperMtrl : public StandardMtrl
	{
		HARE_DECLARE_ABSTRACT_CLASS(WrapperMtrl)
	public:
		WrapperMtrl();
		virtual ~WrapperMtrl();

		virtual StandardMtrl* getSubMtrl()
		{
			return mtrl;
		}

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

	//材质修改器计算纹理坐标

	//平移材质修改器
	class GRAPHICS_API PannerMod : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(PannerMod)
	public:
		PannerMod();
		virtual ~PannerMod();

		virtual void frameMove();
	public:
		PointF   offset;
		PointF   panDirection;
		float      panRate;

		PointF   oscillationPhase;
		PointF   oscillationAmplitude;
		PointF   oscillationRate;
	protected:
		float      startTime;

	};

	//缩放材质修改器
	class GRAPHICS_API ScalerMod : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(ScalerMod)
	public:
		ScalerMod();
		virtual ~ScalerMod();

		virtual void frameMove();
	public:
		PointF	scale;
		PointF	center;

		PointF	oscillationPhase;
		PointF	oscillationAmplitude;
		PointF	oscillationRate;
	protected:
		float		startTime;

	};

	//旋转材质修改器
	class GRAPHICS_API RotatorMod : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(RotatorMod)
	public:
		RotatorMod();
		virtual ~RotatorMod();

		virtual void frameMove();
	public:
		PointF      center;
		float		rotation;
		float		speed;

		float		oscillationPhase;
		float		oscillationAmplitude;
		float		oscillationRate;

	protected:
		float		startTime;

	};


}

#endif
