#ifndef _D3DRENDERSYSTEM_H_
#define _D3DRENDERSYSTEM_H_

class D3DRenderWindow;
class D3DVertexBufferManager;
class D3DIndexBufferManager;
class DeviceManager;


class D3DRenderSystem : public RenderSystem
{
public:
	D3DRenderSystem();
	virtual ~D3DRenderSystem();

	virtual void initalize();//������Ϣ �Ժ��
	void createDevice(D3DRenderWindow* renderWindow);
	void resetDevice();

	virtual void release();

	virtual void beginFrame();
	virtual void render();
	virtual void render(RenderUnit* operation);
	virtual void endFrame();
	virtual void clear(bool z);

	virtual void setRenderTarget(RenderTarget* target);
	virtual void setShaderParams(const ShaderParams& shaderParams);
	virtual void setTextureStage(const TextureStage& textureStage);

	virtual Texture* createTexture();
    virtual void setProjection(float l, float r, float b, float t);
    virtual void prepareCanvasRender();

	LPDIRECT3DDEVICE9 getD3DDevice();

	LPDIRECT3D9 getD3DInterface();

	static const uint32 VERTEX_CAPACITY;

	static const uint32 VERTEX_FVF;
protected:
	void initalizeParam();

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

	D3DPRIMITIVETYPE PrimType; //��ǰ��Ⱦ���� ���� �� �� ������ �ȵ�

	D3DRenderWindow* pPrimaryWindow;

	DeviceManager* pDeviveManager;

	Matrix4 texMat;

    bool bUseIndex;

    D3DXMATRIX MatView;
	D3DXMATRIX MatProj;
};


#endif