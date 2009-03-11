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
		};

		enum ColorAlphaBlendArgument
		{
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

	class GRAPHICS_API Material : public Object
	{
		HARE_DECLARE_ABSTRACT_CLASS(Material)
	public:
		Material();
		virtual ~Material();

		virtual void frameMove() = 0;
		virtual TextureMtrl* getTextureMtrl()
		{
			return NULL;
		}
		
	};

	//纹理材质
	class GRAPHICS_API TextureMtrl : public Material
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
		}
		Texture* getTexture()
		{
			return texture;
		}

		void setUV(f32 left, f32 top, f32 right, f32 bottom)
		{
			uleft = left; 
			uright = right; 
			vtop = top; 
			vbottom = bottom;
		}
		void getUV(f32& left, f32& top, f32& right, f32& bottom)
		{
			left = uleft;
			top  = vtop;
			right= uright;
			bottom=vbottom;
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
	protected:
		Texture::Ptr texture;
		f32 uleft, uright, vtop, vbottom;

		TextureStage textureStage;
	};

	//节点材质基类
	class GRAPHICS_API WrapperMtrl : public Material
	{
		HARE_DECLARE_ABSTRACT_CLASS(WrapperMtrl)
	public:
		WrapperMtrl();
		virtual ~WrapperMtrl();

		virtual void frameMove() = 0;

		virtual Material* getSubMtrl()
		{
			return mtrl;
		}

		virtual void setSubMtrl(Material* m)
		{
			mtrl = m;
		}

		virtual TextureMtrl* getTextureMtrl()
		{
			return mtrl != NULL ? mtrl->getTextureMtrl() : NULL;
		}

	protected:
		Material::Ptr mtrl;
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
		Point<f32> offset;    // 0 - 1
		Point<f32> direction;
		f32        speed;

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
		Point<f32> lt;    // 0 - 1
		Point<f32> scale;

	};

	//旋转材质修改器 以后做 - -! (靠 不用矩阵没法转啊) 
	class GRAPHICS_API RotatorMod : public WrapperMtrl
	{
		HARE_DECLARE_DYNAMIC_CLASS(RotatorMod)
	public:
		RotatorMod();
		virtual ~RotatorMod();

		virtual void frameMove();
	public:
		Point<f32> center; // 0 - 1
		f32 rotation;
		f32 speed;
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