#include "PCH.h"
#include "Material.h"
#include "RenderSystem.h"

namespace hare
{
	HARE_ENUM_BEGIN(TextureStage::WrapMode)
		HARE_ENUM_NAME_VALUE(Wrap,   TextureStage::WM_Wrap)
		HARE_ENUM_NAME_VALUE(Mirror, TextureStage::WM_Mirror)
		HARE_ENUM_NAME_VALUE(Clamp,  TextureStage::WM_Clamp)
		HARE_ENUM_NAME_VALUE(Shadow, TextureStage::WM_Shadow)
	HARE_ENUM_END()

	HARE_ENUM_BEGIN(TextureStage::FliterType)
		HARE_ENUM_NAME_VALUE(Point, TextureStage::FT_Point)
		HARE_ENUM_NAME_VALUE(Line,  TextureStage::FT_Line)
	HARE_ENUM_END()

	HARE_IMPLEMENT_DYNAMIC_CLASS(TextureStage, Object, 0)
	{
		HARE_ENUM(wrapModeU,  uint8, WrapMode)
		HARE_ENUM(wrapModeV,  uint8, WrapMode)
		HARE_ENUM(fliterType, uint8, FliterType)
	}

	TextureStage::TextureStage()
		:ColorBlendOP(CABO_Modulate)
		,ColorBlendArg1(CABA_Texture)
		,ColorBlendArg2(CBBA_Current)
		,AlphaBlendOP(CABO_Modulate)
		,AlphaBlendArg1(CABA_Texture)
		,AlphaBlendArg2(CBBA_Current)
		,wrapModeU(WM_Mirror)
		,wrapModeV(WM_Mirror)
		,fliterType(FT_Point)
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
		|| fliterType         != right.fliterType){
			return true;
		}

		return false;
	}

	bool TextureStage::operator == (const TextureStage& right)
	{
		return !(*this != right);
	}

	TextureStage& TextureStage::operator = (const TextureStage& right)
	{
		ColorBlendOP   = right.ColorBlendOP;
		ColorBlendArg1 = right.ColorBlendArg1;
		ColorBlendArg2 = right.ColorBlendArg2;
		AlphaBlendOP   = right.AlphaBlendOP;
		AlphaBlendArg1 = right.AlphaBlendArg1;
		AlphaBlendArg2 = right.AlphaBlendArg2;
		wrapModeU      = right.wrapModeU;
		wrapModeV      = right.wrapModeV;
		fliterType     = right.fliterType;

		return *this;
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
	{
		HARE_META_F(fileName, String, propFSUrl)
		HARE_OBJ(textureStage, TextureStage)
	}

	TextureMtrl::TextureMtrl()
	{
		textureStage = new TextureStage;
		frameMove();
	}

	TextureMtrl::~TextureMtrl()
	{

	}

	void TextureMtrl::frameMove()
	{
		texMat = Matrix4::IDENTITY;
	}

	void TextureMtrl::postLoaded()
	{
		if (!fileName.empty()){
			texture = RenderSystem::getSingletonPtr()->createTexture();
			texture->createFromFile(fileName);
		}
	}

	void TextureMtrl::postEdited(Attribute *attr)
	{
		postLoaded();
	}

	//节点包裹材质
	HARE_IMPLEMENT_ABSTRACT_CLASS(WrapperMtrl, StandardMtrl, 0)
	{
		HARE_OBJ_F(mtrl, StandardMtrl, propHide)
	}

	WrapperMtrl::WrapperMtrl()
		:mtrl(NULL)
	{

	}

	WrapperMtrl::~WrapperMtrl()
	{

	}

	//平移材质修改器
	HARE_IMPLEMENT_DYNAMIC_CLASS(PannerMod, WrapperMtrl, 0)
	{
        HARE_META(offset, PointF)
		HARE_META(panDirection, PointF)
		HARE_META(panRate, float)
		HARE_META(oscillationPhase, PointF)
		HARE_META(oscillationAmplitude, PointF)
		HARE_META(oscillationRate, PointF)
	}

	PannerMod::PannerMod()
        :offset(PointF::ZERO)
        ,panDirection(PointF::ZERO)
        ,panRate(0)
        ,oscillationPhase(PointF::ZERO)
        ,oscillationAmplitude(PointF::ZERO)
        ,oscillationRate(PointF::ZERO)
		,startTime(0)
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

			float detTime = getTime() - startTime;

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
	{
        HARE_META(scale, PointF)
        HARE_META(center, PointF)
        HARE_META(oscillationPhase, PointF)
        HARE_META(oscillationAmplitude, PointF)
		HARE_META(oscillationRate, PointF)
	}

	ScalerMod::ScalerMod()
		:scale(1, 1)
		,center(PointF::ZERO)
		,oscillationPhase(PointF::ZERO)
		,oscillationAmplitude(PointF::ZERO)
		,oscillationRate(PointF::ZERO)
        ,startTime(0)
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

			float detTime = getTime() - startTime;

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
	{
        HARE_META(center, PointF)
		HARE_META(rotation, float)
		HARE_META(speed, float)
		HARE_META(oscillationPhase, float)
		HARE_META(oscillationAmplitude, float)
		HARE_META(oscillationRate, float)
	}

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

			float detTime = getTime() - startTime;

			float cur = oscillationRate*detTime + oscillationPhase;
			float degree;
			degree = rotation + MathUtil::sinf(MathUtil::toRadian(cur))*(oscillationAmplitude/2.f) + speed*detTime;

			float c = MathUtil::cosf(MathUtil::toRadian(degree));
			float s = MathUtil::sinf(MathUtil::toRadian(degree));

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

}
