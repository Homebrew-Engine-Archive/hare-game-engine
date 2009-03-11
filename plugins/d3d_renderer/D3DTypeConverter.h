#ifndef D3DTYPECONVERTER
#define D3DTYPECONVERTER

namespace hare_d3d
{
	class D3DTypeConverter
	{
	public:
		static D3DBLENDOP   toD3DSceneBlendOperation(ShaderParams::SceneBlendOperation sceneOP);
		static D3DBLEND     toD3DSceneBlendArgument(ShaderParams::SceneBlendArgument sceneArg);
		static D3DTEXTUREOP toD3DTextureOperation(TextureStage::ColorAlphaBlendOperation texOP);
		static u32          toD3DTextureArgument(TextureStage::ColorAlphaBlendArgument texArg);
		static D3DTEXTUREADDRESS toD3DTextureAddress(TextureStage::WrapMode wrapMod);
		static D3DFORMAT    toD3DFormat(HarePixelFormat hpf);
		static D3DPRIMITIVETYPE toD3DPrimtiveType(RenderOperation::RenderOperationType primtype);
	};
}

#endif 