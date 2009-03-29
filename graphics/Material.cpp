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

	//节点材质
	HARE_IMPLEMENT_ABSTRACT_CLASS(StandardMtrl, Material, 0)
	{}

	StandardMtrl::StandardMtrl()
	{

	}

	StandardMtrl::~StandardMtrl()
	{

	}

	//纹理材质
	HARE_IMPLEMENT_DYNAMIC_CLASS(TextureMtrl, StandardMtrl, 0)
	{}

	TextureMtrl::TextureMtrl()
	{
		frameMove();
	}

	TextureMtrl::~TextureMtrl()
	{

	}

	void TextureMtrl::frameMove()
	{
		texMat = Matrix4::IDENTITY;
	}

	//节点包裹材质 
	HARE_IMPLEMENT_ABSTRACT_CLASS(WrapperMtrl, StandardMtrl, 0)
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
		:startTime(0)
		,offset(PointF::ZERO)
		,panDirection(PointF::ZERO)
		,panRate(0)
		,oscillationPhase(PointF::ZERO)
		,oscillationAmplitude(PointF::ZERO)
		,oscillationRate(PointF::ZERO)
	{

	}

	PannerMod::~PannerMod()
	{

	}

	void PannerMod::frameMove()
	{
		if (mtrl)
			mtrl->frameMove();

		TextureMtrl* texMtrl = getTextureMtrl();

		if (texMtrl){
			if (startTime == 0){
				startTime = getTime();
			}

			f32 detTime = getTime() - startTime;

			PointF curAngle;
			curAngle.x = oscillationRate.x*detTime + oscillationPhase.x;
			curAngle.y = oscillationRate.y*detTime + oscillationPhase.y;

			PointF aniOffset;
			aniOffset.x = MathUtil::sinf(MathUtil::toRadian(curAngle.x))*(oscillationAmplitude.x/2.f);
			aniOffset.y = MathUtil::sinf(MathUtil::toRadian(curAngle.y))*(oscillationAmplitude.y/2.f);

			PointF cur = panDirection * panRate * detTime + aniOffset;

			cur.x = MathUtil::modff(cur.x);
			cur.y = MathUtil::modff(cur.y);

			Matrix4 mat(Matrix4::IDENTITY);
			mat._13 = -cur.x - offset.x;
			mat._23 = -cur.y - offset.y;

			texMtrl->texMat = mat * texMtrl->texMat;
		}
	}

	//缩放材质修改器
	HARE_IMPLEMENT_DYNAMIC_CLASS(ScalerMod, WrapperMtrl, 0)
	{}

	ScalerMod::ScalerMod()
		:startTime(0)
		,scale(1, 1)
		,center(PointF::ZERO)
		,oscillationPhase(PointF::ZERO)
		,oscillationAmplitude(PointF::ZERO)
		,oscillationRate(PointF::ZERO)
	{

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
			if (startTime == 0){
				startTime = getTime();
			}

			f32 detTime = getTime() - startTime;

			PointF curAngle;
			curAngle.x = oscillationRate.x*detTime + oscillationPhase.x;
			curAngle.y = oscillationRate.y*detTime + oscillationPhase.y;

			PointF aniScale;
			aniScale.x = MathUtil::sinf(MathUtil::toRadian(curAngle.x))*(oscillationAmplitude.x/2.f);
			aniScale.y = MathUtil::sinf(MathUtil::toRadian(curAngle.y))*(oscillationAmplitude.y/2.f);

			// move to center.
			Matrix4 mat;
			mat = Matrix4::IDENTITY;

			mat._13 = -center.x;
			mat._23 = -center.y;

			texMtrl->texMat = mat * texMtrl->texMat;		

			mat = Matrix4::IDENTITY;
			mat._11 = 1.f/(scale.x + aniScale.x);
			mat._22 = 1.f/(scale.y + aniScale.y);

			mat._13 = center.x;
			mat._23 = center.y;

			texMtrl->texMat = mat * texMtrl->texMat;
		}
	}

	//旋转材质修改器
	HARE_IMPLEMENT_DYNAMIC_CLASS(RotatorMod, WrapperMtrl, 0)
	{}

	RotatorMod::RotatorMod()
		:center(0, 0)
		,rotation(0)
		,speed(0)
		,oscillationPhase(0)
		,oscillationAmplitude(0)
		,oscillationRate(0)
		,startTime(0)
	{

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
			if (startTime == 0){
				startTime = getTime();
			}

			f32 detTime = getTime() - startTime;

			f32 cur = oscillationRate*detTime + oscillationPhase;
			f32 degree;
			degree = rotation + MathUtil::sinf(MathUtil::toRadian(cur))*(oscillationAmplitude/2.f) + speed*detTime;

			f32 c = MathUtil::cosf(MathUtil::toRadian(degree));
			f32 s = MathUtil::sinf(MathUtil::toRadian(degree));

			Matrix4 mat;
			mat = Matrix4::IDENTITY;

			mat._13 = -center.x;
			mat._23 = -center.y;

			texMtrl->texMat = mat * texMtrl->texMat;		

			mat = Matrix4::IDENTITY;

			mat._11 = c;
			mat._12 = -s;
			mat._21 = s;
			mat._22 = c;

			mat._13 = center.x;
			mat._23 = center.y;

			texMtrl->texMat = mat * texMtrl->texMat;
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