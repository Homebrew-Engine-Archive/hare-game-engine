#ifndef MATERIAL
#define MATERIAL

//材质
#include "GraphicsPrerequisites.h"
#include "Texture.h"

namespace hare_graphics
{
	class GRAPHICS_API TextureStage
	{
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

		enum LODSet
		{
			LS_Interface,	    //没有minimap
			LS_HighQuality,		//
		};

		TextureStage();
		~TextureStage(){}

		ColorAlphaBlendOperation ColorBlendOP;
		ColorAlphaBlendArgument  ColorBlendArg1;
		ColorAlphaBlendArgument  ColorBlendArg2;

		ColorAlphaBlendOperation AlphaBlendOP;
		ColorAlphaBlendArgument  AlphaBlendArg1;
		ColorAlphaBlendArgument  AlphaBlendArg2;

		WrapMode wrapModeU;
		WrapMode wrapModeV;

		LODSet   lodSet;

		bool operator != (const TextureStage& right);
		bool operator == (const TextureStage& right);
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

		void setWrapModU(TextureStage::WrapMode modeU)
		{
			textureStage.wrapModeU = modeU;
		}
		void setWrapModV(TextureStage::WrapMode modeV)
		{
			textureStage.wrapModeV = modeV;
		}

		TextureStage::WrapMode getWrapModeU()
		{
			return textureStage.wrapModeU;
		}

		TextureStage::WrapMode getWrapModeV()
		{
			return textureStage.wrapModeV;
		}

		void setLodSet(TextureStage::LODSet lod)
		{
			textureStage.lodSet = lod;
		}

		TextureStage::LODSet getLodSet()
		{
			return textureStage.lodSet;
		}

		void setTextureStage(const TextureStage& ts)
		{
			textureStage = ts;
		}

		const TextureStage& getTextureStage()
		{
			return textureStage;
		}

		virtual void postLoaded();

		virtual void postEdited(Attribute *attr);

	protected:
		Texture::Ptr texture;
		TextureStage textureStage;
	
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
		f32      panRate; 

		PointF   oscillationPhase;
		PointF   oscillationAmplitude;
		PointF   oscillationRate;
	protected:
		f32      startTime;

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
		f32		startTime;

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
		PointF	center;
		f32		rotation;
		f32		speed;

		f32		oscillationPhase;
		f32		oscillationAmplitude;
		f32		oscillationRate;

	protected:
		f32		startTime;

	};

	//动画材质修改器
	
	//动画材质修改器单元
	class GRAPHICS_API AnimModUnit : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(AnimModUnit)
	public:
		AnimModUnit();
		virtual ~AnimModUnit();

		virtual void frameMove();
	public:
		f32 frameTime;
	};


	class GRAPHICS_API AnimMod : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(AnimMod)
	public:
		AnimMod();
		virtual ~AnimMod();

		virtual void frameMove();

		void addFrame(AnimModUnit* animModUnit);

	public:
		AnimModUnit::Array animMtrlList;  
		int curAnimMtrlID;

	};



	class GRAPHICS_API AnimMtrl : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(AnimMtrl)
	public:
		AnimMtrl();
		virtual ~AnimMtrl();

		virtual void frameMove();
	public:
	};


}

#endif
