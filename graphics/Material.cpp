#include "PCH.h"
#include "Material.h"


namespace hare_graphics
{
	TextureStage::TextureStage()
		:ColorBlendOP(CABO_Modulate)
		,ColorBlendArg1(CABA_Texture)
		,ColorBlendArg2(CABA_Diffuse)
		,AlphaBlendOP(CABO_Modulate)
		,AlphaBlendArg1(CABA_Texture)
		,AlphaBlendArg2(CABA_Diffuse)
		,wrapModeU(WM_Wrap)
		,wrapModeV(WM_Wrap)
		,lodSet(LS_Interface)
	{

	}

	bool TextureStage::operator != (const TextureStage& right)
	{
		if (ColorBlendOP  != right.ColorBlendOP 
		|| ColorBlendArg1 != right.ColorBlendArg1
		|| ColorBlendArg2 != right.ColorBlendArg2
		|| AlphaBlendOP   != right.AlphaBlendOP
		|| AlphaBlendArg1 != right.AlphaBlendArg1
		|| AlphaBlendArg2 != right.AlphaBlendArg2
		|| wrapModeU      != right.wrapModeU
		|| wrapModeV      != right.wrapModeV
		|| lodSet         != right.lodSet){
			return true;
		}

		return false;
	}

	bool TextureStage::operator == (const TextureStage& right)
	{
		return !(*this != right);
	}

	//材质基类
	HARE_IMPLEMENT_ABSTRACT_CLASS(Material, Object, 0)
	{}

	Material::Material()
	{

	}

	Material::~Material()
	{

	}

	//纹理材质
	HARE_IMPLEMENT_DYNAMIC_CLASS(TextureMtrl, Material, 0)
	{}

	TextureMtrl::TextureMtrl()
	{
		textureStage.AlphaBlendOP = TextureStage::CABO_Modulate;
		textureStage.AlphaBlendArg1 = TextureStage::CABA_Texture;
		textureStage.AlphaBlendArg2 = TextureStage::CABA_Diffuse;
		textureStage.ColorBlendOP = TextureStage::CABO_Modulate;
		textureStage.ColorBlendArg1 = TextureStage::CABA_Texture;
		textureStage.ColorBlendArg2 = TextureStage::CABA_Diffuse;
		textureStage.wrapModeU = TextureStage::WM_Wrap;
		textureStage.wrapModeV = TextureStage::WM_Wrap;
		textureStage.lodSet = TextureStage::LS_Interface;
		frameMove();
	}

	TextureMtrl::~TextureMtrl()
	{

	}

	void TextureMtrl::frameMove()
	{
		uleft = 0; 
		uright= 1; 
		vtop  = 0;
		vbottom = 1;
	}

	//结点材质 
	HARE_IMPLEMENT_ABSTRACT_CLASS(WrapperMtrl, Material, 0)
	{}

	WrapperMtrl::WrapperMtrl()
		:mtrl(NULL)
	{

	}

	WrapperMtrl::~WrapperMtrl()
	{

	}

	//平移材质修改器
	HARE_IMPLEMENT_DYNAMIC_CLASS(PannerMod, WrapperMtrl, 0)
	{}

	PannerMod::PannerMod()
	{
		offset.x = 0;    // 0 - 1
		offset.y = 0;
		direction.x = 0;
		direction.y = 0;
		speed = 0;
	}

	PannerMod::~PannerMod()
	{

	}

	void PannerMod::frameMove()
	{
		static f32 tmpOffsetX = 0;
		static f32 tmpOffsetY = 0;
		if (mtrl)
			mtrl->frameMove();

		TextureMtrl* texMtrl = getTextureMtrl();

		if (texMtrl){
			f32 uleft, uright, vtop, vbottom;
			texMtrl->getUV(uleft, uright, vtop, vbottom);
			
			tmpOffsetX += direction.x * speed * getTimer().getDeltaTime() + offset.x;
			tmpOffsetY += direction.y * speed * getTimer().getDeltaTime() + offset.y;

			uleft += tmpOffsetX; 
			uright+= tmpOffsetX; 
			vtop  += tmpOffsetY;
			vbottom+=tmpOffsetY;

			texMtrl->setUV(uleft, uright, vtop, vbottom);
		}
	}

	//缩放材质修改器
	HARE_IMPLEMENT_DYNAMIC_CLASS(ScalerMod, WrapperMtrl, 0)
	{}

	ScalerMod::ScalerMod()
	{
		lt.x = 0;    // 0 - 1
		lt.y = 0;
		scale.x = 1;
		scale.y = 1;
	}

	ScalerMod::~ScalerMod()
	{

	}

	void ScalerMod::frameMove()
	{
		if (mtrl)
			mtrl->frameMove();
	
		TextureMtrl* texMtrl = getTextureMtrl();

		if (texMtrl){
			f32 uleft, uright, vtop, vbottom;
			texMtrl->getUV(uleft, uright, vtop, vbottom);	

			f32 tmpScaleX = 1.0f / scale.x;
			f32 tmpScaleY = 1.0f / scale.y;

			uleft += lt.x;
			vtop += lt.y;

			uright = uleft + tmpScaleX;
			vbottom= vtop  + tmpScaleY;

			texMtrl->setUV(uleft, uright, vtop, vbottom);	
		}
	}

	//旋转材质修改器
	HARE_IMPLEMENT_DYNAMIC_CLASS(RotatorMod, WrapperMtrl, 0)
	{}

	RotatorMod::RotatorMod()
	{
		center.x = 0; // 0 - 1
		center.y = 0;
		rotation = 0;
		speed = 0;
	}

	RotatorMod::~RotatorMod()
	{

	}

	void RotatorMod::frameMove()
	{
		if (mtrl)
			mtrl->frameMove();
	
		TextureMtrl* texMtrl = getTextureMtrl();

		if (texMtrl){
			f32 uleft, uright, vtop, vbottom;
			texMtrl->getUV(uleft, uright, vtop, vbottom);	

			f32 tx1, ty1, tx2, ty2;
			//f32 cost, sint;

			tx1 = -center.x;
			ty1 = -center.y;
			tx2 = 1.0f - center.x;
			ty2 = 1.0f - center.y;
	
			f32 tmpRotation = speed * getTimer().getDeltaTime() + rotation;

			if (tmpRotation != 0.0f){	

			}else{
				
			}

			texMtrl->setUV(uleft, uright, vtop, vbottom);	
		}
	}

	//动画材质修改器
	HARE_IMPLEMENT_DYNAMIC_CLASS(AnimModUnit, WrapperMtrl, 0)
	{}

	AnimModUnit::AnimModUnit()
		:frameTime(0)
	{

	}

	AnimModUnit::~AnimModUnit()
	{

	}

	void AnimModUnit::frameMove()
	{
		if (mtrl)
			mtrl->frameMove();
	}



	HARE_IMPLEMENT_DYNAMIC_CLASS(AnimMod, WrapperMtrl, 0)
	{}

	AnimMod::AnimMod()
		:curAnimMtrlID(0)
	{

	}

	AnimMod::~AnimMod()
	{

	}

	void AnimMod::frameMove()
	{
		static f32 frameStartTime = 0;

		if (!mtrl)
			return;

		mtrl->frameMove();
		frameStartTime += getTimer().getDeltaTime();

		if (frameStartTime >= animMtrlList[curAnimMtrlID]->frameTime){
			curAnimMtrlID = (curAnimMtrlID + 1) % animMtrlList.size();
			mtrl = animMtrlList[curAnimMtrlID];
			frameStartTime = 0;
		}
	}

	void AnimMod::addFrame(AnimModUnit* animModUnit)
	{
		animMtrlList.push_back(animModUnit);
		curAnimMtrlID = 0;
		mtrl = animMtrlList[curAnimMtrlID]->getSubMtrl();
	}

	HARE_IMPLEMENT_DYNAMIC_CLASS(AnimMtrl, WrapperMtrl, 0)
	{}

	AnimMtrl::AnimMtrl()
	{

	}

	AnimMtrl::~AnimMtrl()
	{

	}

	void AnimMtrl::frameMove()
	{

	}



}