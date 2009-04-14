#include "PCH.h"
#include "Shader.h"

namespace hare_graphics
{
	HARE_ENUM_BEGIN(ShaderParams::SceneBlendOperation)
		HARE_ENUM_NAME_VALUE(Add,         ShaderParams::SBO_Add)
		HARE_ENUM_NAME_VALUE(Revsubtract, ShaderParams::SBO_Revsubtract)
		HARE_ENUM_NAME_VALUE(Min,         ShaderParams::SBO_Min)
		HARE_ENUM_NAME_VALUE(Max,         ShaderParams::SBO_Max)
	HARE_ENUM_END()

	HARE_ENUM_BEGIN(ShaderParams::SceneBlendArgument)
		HARE_ENUM_NAME_VALUE(Srcalpha,    ShaderParams::SBA_Srcalpha)
		HARE_ENUM_NAME_VALUE(Invsrcalpha, ShaderParams::SBA_Invsrcalpha)
		HARE_ENUM_NAME_VALUE(One,         ShaderParams::SBA_One)
		HARE_ENUM_NAME_VALUE(Zero,        ShaderParams::SBA_Zero)
	HARE_ENUM_END()

	HARE_IMPLEMENT_DYNAMIC_CLASS(ShaderParams, Object, 0)
	{
		HARE_META(AlphaBlendEnable, bool) 
		HARE_META(AlphaTestEnable,  bool) 
		HARE_META(AlphaRef,         uint8)
		HARE_ENUM(SceneBlendOP,     uint8, SceneBlendOperation)
		HARE_ENUM(SceneBlendSrcArg, uint8, SceneBlendArgument)
		HARE_ENUM(SceneBlendDesArg, uint8, SceneBlendArgument)
	}

	ShaderParams::ShaderParams()
		:AlphaBlendEnable(false)
		,SceneBlendOP(SBO_Add)
		,SceneBlendSrcArg(SBA_Srcalpha)
		,SceneBlendDesArg(SBA_Invsrcalpha)
		,AlphaTestEnable(true)
		,AlphaRef(1)
	{

	}

	bool ShaderParams::operator != (const ShaderParams& right)
	{
		if (AlphaBlendEnable != right.AlphaBlendEnable
		|| SceneBlendOP      != right.SceneBlendOP
		|| SceneBlendSrcArg  != right.SceneBlendSrcArg
		|| SceneBlendDesArg  != right.SceneBlendDesArg
		|| AlphaTestEnable   != right.AlphaTestEnable
		|| AlphaRef          != right.AlphaRef){
			return true;
		}

		return false;
	}

	bool ShaderParams::operator == (const ShaderParams& right)
	{
		return !((*this) != right);
	}

	ShaderParams& ShaderParams::operator = (const ShaderParams& right)
	{
		AlphaBlendEnable = right.AlphaBlendEnable;
		SceneBlendOP     = right.SceneBlendOP;
		SceneBlendSrcArg = right.SceneBlendSrcArg;
		SceneBlendDesArg = right.SceneBlendDesArg;
		AlphaTestEnable  = right.AlphaTestEnable;
		AlphaRef         = right.AlphaRef;

		return *this;
	}


	HARE_IMPLEMENT_ABSTRACT_CLASS(Shader, Material, 0)
	{
		HARE_OBJ(shaderParams, ShaderParams)
	}

	Shader::Shader()
	{
		shaderParams = new ShaderParams;
	}

	Shader::~Shader()
	{

	}

	HARE_IMPLEMENT_DYNAMIC_CLASS(SimpleShader, Shader, 0)
	{
		HARE_OBJ(mtrl, StandardMtrl)
	}

	SimpleShader::SimpleShader()
	{

	}

	SimpleShader::~SimpleShader()
	{

	}


	HARE_IMPLEMENT_DYNAMIC_CLASS(ParticleShader, Shader, 0)
	{
		HARE_OBJ(mtrl, StandardMtrl)
	}

	ParticleShader::ParticleShader()
	{
		shaderParams->AlphaBlendEnable = true;
		shaderParams->AlphaTestEnable = false;
	}

	ParticleShader::~ParticleShader()
	{

	}
}
