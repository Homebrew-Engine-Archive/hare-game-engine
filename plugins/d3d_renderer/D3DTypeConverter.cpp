#include "PCH.h"
#include "D3DTypeConverter.h"


D3DBLENDOP D3DTypeConverter::toD3DSceneBlendOperation(ShaderParams::SceneBlendOperation sceneOP)
{
	D3DBLENDOP d3dBlendOP;
	switch(sceneOP)
	{
	case ShaderParams::SBO_Add:
		d3dBlendOP = D3DBLENDOP_ADD;
		break;
	case ShaderParams::SBO_Max:
		d3dBlendOP = D3DBLENDOP_MAX;
		break;
	case ShaderParams::SBO_Min:
		d3dBlendOP = D3DBLENDOP_MIN;
		break;
	case ShaderParams::SBO_Revsubtract:
		d3dBlendOP = D3DBLENDOP_REVSUBTRACT;
		break;
	default:
		assert(false);
	}
	return d3dBlendOP;
}

D3DBLEND D3DTypeConverter::toD3DSceneBlendArgument(ShaderParams::SceneBlendArgument sceneArg)
{
	D3DBLEND d3dBlendArg;
	switch(sceneArg)
	{
	case ShaderParams::SBA_Srcalpha:
		d3dBlendArg = D3DBLEND_SRCALPHA;
		break;
	case ShaderParams::SBA_Invsrcalpha:
		d3dBlendArg = D3DBLEND_INVSRCALPHA;
		break;
	case ShaderParams::SBA_One:
		d3dBlendArg = D3DBLEND_ONE;
		break;
	case ShaderParams::SBA_Zero:
		d3dBlendArg = D3DBLEND_ZERO;
		break;
	default:
		assert(false);
	}
	return d3dBlendArg;
}

D3DTEXTUREOP D3DTypeConverter::toD3DTextureOperation(TextureStage::ColorAlphaBlendOperation texOP)
{
	D3DTEXTUREOP d3dTextureOp;
	switch(texOP)
	{
	case TextureStage::CABO_Modulate:
		d3dTextureOp = D3DTOP_MODULATE;
		break;
	case TextureStage::CABO_Select1:
		d3dTextureOp = D3DTOP_SELECTARG1;
		break;
	case TextureStage::CABO_Add:
		d3dTextureOp = D3DTOP_ADD;
		break;
	case TextureStage::CABO_BlendTexAlpha:
		d3dTextureOp = D3DTOP_BLENDTEXTUREALPHA;
		break;
	default:
		assert(false);
	}
	return d3dTextureOp;
}

uint32 D3DTypeConverter::toD3DTextureArgument(TextureStage::ColorAlphaBlendArgument texArg)
{
	uint32 d3dTextureArg;
	switch(texArg)
	{
	case TextureStage::CBBA_Current:
		d3dTextureArg = D3DTA_CURRENT;
		break;
	case TextureStage::CABA_Texture:
		d3dTextureArg = D3DTA_TEXTURE;
		break;
	case TextureStage::CABA_Diffuse:
		d3dTextureArg = D3DTA_DIFFUSE;
		break;
	default:
		assert(false);
	}
	return d3dTextureArg;
}

D3DTEXTUREADDRESS D3DTypeConverter::toD3DTextureAddress(TextureStage::WrapMode wrapMod)
{
	D3DTEXTUREADDRESS d3dTextureAddress;
	switch(wrapMod)
	{
	case TextureStage::WM_Wrap:
		d3dTextureAddress = D3DTADDRESS_WRAP;
		break;
	case TextureStage::WM_Shadow:
		d3dTextureAddress = D3DTADDRESS_BORDER;
		break;
	case TextureStage::WM_Mirror:
		d3dTextureAddress = D3DTADDRESS_MIRROR;
		break;
	case TextureStage::WM_Clamp:
		d3dTextureAddress = D3DTADDRESS_CLAMP;
		break;
	default:
		assert(false);
	}
	return d3dTextureAddress;
}

D3DFORMAT D3DTypeConverter::toD3DFormat(HarePixelFormat hpf)
{
	D3DFORMAT d3dfmt;
	switch(hpf)
	{
	case HPF_BYTE_LA:
		d3dfmt = D3DFMT_A8L8;//假设小地址结尾
		break;
	case HPF_A8R8G8B8:
		d3dfmt = D3DFMT_A8R8G8B8;
		break;
	default:
		d3dfmt = D3DFMT_UNKNOWN;
		assert(false);
	}
	return d3dfmt;
}

D3DPRIMITIVETYPE D3DTypeConverter::toD3DPrimtiveType(RenderUnit::RenderOperationType primtype)
{
	D3DPRIMITIVETYPE retPrimType;
	switch(primtype)
	{
	case RenderUnit::ROT_POINT_LIST:     retPrimType = D3DPT_POINTLIST;    break;
	case RenderUnit::ROT_LINE_LIST:      retPrimType = D3DPT_LINELIST;     break;
	case RenderUnit::ROT_LINE_STRIP:     retPrimType = D3DPT_LINESTRIP;    break;
	case RenderUnit::ROT_TRIANGLE_LIST:  retPrimType = D3DPT_TRIANGLELIST; break;
	case RenderUnit::ROT_TRIANGLE_STRIP: retPrimType = D3DPT_TRIANGLESTRIP;break;
	case RenderUnit::ROT_TRIANGLE_FAN:   retPrimType = D3DPT_TRIANGLEFAN;  break;
    case RenderUnit::ROT_QUAD_LIST:      retPrimType = D3DPT_TRIANGLELIST; break;
	default: assert(false);
	}

	return retPrimType;
}

D3DMATRIX D3DTypeConverter::toD3DMatrix(const Matrix4& mat)
{
	D3DXMATRIX d3dmat;
	D3DXMatrixTranspose(&d3dmat,(const D3DXMATRIX*)(&mat));

	return d3dmat;
}

