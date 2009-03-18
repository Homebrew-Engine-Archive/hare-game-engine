#include "PCH.h"
#include "Shader.h"

namespace hare_graphics
{
	ShaderParams::ShaderParams()
		:AlphaBlendEnable(true)
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

	HARE_IMPLEMENT_ABSTRACT_CLASS(Shader, Object, 0)
	{}

	Shader::Shader()
	{

	}

	Shader::~Shader()
	{

	}

	HARE_IMPLEMENT_DYNAMIC_CLASS(SimpleShader, Shader, 0)
	{}

	SimpleShader::SimpleShader()
	{

	}

	SimpleShader::~SimpleShader()
	{

	}

}