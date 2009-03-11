#ifndef D3DRENDERSYSTEM
#define D3DRENDERSYSTEM

class D3DRenderWindow;
class D3DVertexBufferManager;
class D3DIndexBufferManager;
class DeviceManager;


namespace hare_d3d
{
	class D3DRenderSystem : public RenderSystem
	{
	public:
		D3DRenderSystem();
		virtual ~D3DRenderSystem();

		virtual void initalize();//配置信息 以后加
		void createDevice(D3DRenderWindow* renderWindow);

		virtual void resetDevice();
		virtual void release();

		virtual void beginFrame();
		virtual void render();
		virtual void render(RenderOperation* operation);
		virtual void endFrame();
		virtual void clear();

		virtual void setRenderTarget(RenderTarget* target);
		virtual void setShaderParams(const ShaderParams& shaderParams);
		virtual void setTextureStage(const TextureStage& textureStage);

		virtual Texture* createTexture();

		LPDIRECT3DDEVICE9 getD3DDevice();

		LPDIRECT3D9 getD3DInterface();

		static const u32 VERTEX_CAPACITY;

		static const u32 VERTEX_FVF;
	private:

		LPDIRECT3D9 pD3DInterface;

		LPDIRECT3DDEVICE9 pD3DDevice;

		D3DCAPS9 d3dCaps;

		D3DVertexBufferManager* pVertexBufferManager; 

		D3DIndexBufferManager* pIndexBufferManager;

		ShaderParams curShaderParams;

		TextureStage curTextureStage;

		LPDIRECT3DTEXTURE9 curRenderTexture;

		bool bAnisotropicFilter;

		D3DPRIMITIVETYPE PrimType; //当前渲染类型 例如 点 线 三角形 等等

		D3DRenderWindow* pPrimaryWindow;

		DeviceManager* pDeviveManager;
	};
}

#endif