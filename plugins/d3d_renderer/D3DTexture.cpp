#include "PCH.h"
#include "D3DTexture.h"
#include "D3DRenderSystem.h"
#include "D3DTypeConverter.h"
#include "D3DRenderWindow.h"


namespace hare_d3d
{
	D3DTexture::D3DTexture()
		:d3dTexture(NULL)
		,pDepthStencilSurface(NULL)
	{
		pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
		assert(pD3DDevice);
	}

	D3DTexture::~D3DTexture()
	{
		release();
	}

	LPDIRECT3DTEXTURE9 D3DTexture::getD3DTexture()
	{
		return d3dTexture;
	}

	void D3DTexture::active()
	{
		assert(bIsRenderable);
		assert(pD3DDevice);

		D3DRenderSystem::getSingletonPtr()->beginFrame();

		HRESULT hr;

		LPDIRECT3DSURFACE9 sur;
		hr = d3dTexture->GetSurfaceLevel(0, &sur);
		if (FAILED(hr))
			assert(false);

		hr = pD3DDevice->SetRenderTarget(0, sur); 
		if (FAILED(hr))
			assert(false);

		hr = pD3DDevice->SetDepthStencilSurface(pDepthStencilSurface);
		if (FAILED(hr))
			assert(false);

		SAFE_RELEASE(sur);

		D3DRenderSystem::getSingletonPtr()->clear();

		//��������ͶӰ����
		D3DXMATRIX matTEMP;

		D3DXMatrixScaling(&MatProj, 1.0f, -1.0f, 1.0f);
		D3DXMatrixTranslation(&matTEMP, -0.5f, projectionHeight + 0.5f, 0.0f);
		D3DXMatrixMultiply(&MatProj, &MatProj, &matTEMP);

		D3DXMatrixOrthoOffCenterLH(&matTEMP, 0, projectionWidth, 0, 
			projectionHeight, 0.0f, 1.0f);//����ͶӰ

		D3DXMatrixMultiply(&MatProj, &MatProj, &matTEMP);
		D3DXMatrixIdentity(&MatView);

		pD3DDevice->SetTransform(D3DTS_VIEW, &MatView);
		pD3DDevice->SetTransform(D3DTS_PROJECTION, &MatProj);

	}

	void D3DTexture::inactive()
	{
		assert(bIsRenderable);

		D3DRenderSystem::getSingletonPtr()->render();

		D3DRenderSystem::getSingletonPtr()->endFrame();
	}

	void D3DTexture::beforeResetDevice()
	{
		assert(bIsRenderable);
		release();

	}

	void D3DTexture::afterResetDevice()
	{
		assert(bIsRenderable);
		doCreate();
	}

	void D3DTexture::upload(const Image &img, u32 destX, u32 destY)
	{
		assert(img.getImageData() != NULL && destX >= 0 && destY >= 0 && destX < width && destY < height);

		int desWidth = min(img.getWidth(), width - destX);
		int desHeight= min(img.getHeight(), height- destY);

		RECT srcRect = { 0, 0, img.getWidth(), img.getHeight() };
		RECT desRect = { destX, destY, destX + desWidth, destY + desHeight };

		HRESULT hr;
		LPDIRECT3DSURFACE9 sur;
		hr = d3dTexture->GetSurfaceLevel(0, &sur);
		if (FAILED(hr))
			assert(false);

		hr = D3DXLoadSurfaceFromMemory(
			sur, 
			NULL, 
			&desRect, 
			img.getImageData(), 
			D3DTypeConverter::toD3DFormat(img.getPixelFormat()), 
			img.getRowStride(), 
			NULL, 
			&srcRect, 
			D3DX_FILTER_POINT, 
			0);

		if (FAILED(hr))
			assert(false);

		SAFE_RELEASE(sur);

	}

	void D3DTexture::download(Image &img, const Rect<int>* rc)
	{
		
	}

	void D3DTexture::reCreate()
	{
		//ע���Լ����豸�������
		if (bIsRenderable){
			DeviceManager::getSingletonPtr()->registerDeviceObject(this);
		}

		assert(doCreate());
	}

	void D3DTexture::release()
	{
		//�Ƴ��Լ����豸�������
		if (bIsRenderable){
			DeviceManager::getSingletonPtr()->unregisterDeviceObject(this);
		}
		
		if (bIsRenderable){	
			SAFE_RELEASE(pDepthStencilSurface)	
		}

		SAFE_RELEASE(d3dTexture)
	}

	bool D3DTexture::doCreate()
	{
	
		if (!bFromImage){
			if (D3DTypeConverter::toD3DFormat(texPixelFormat) == D3DFMT_UNKNOWN)
				return false;

			if (FAILED(pD3DDevice->CreateTexture(//�˺����������width��heightΪ2���ݴη����򴴽�ʧ�� �ڵ���createRenderToTex����ʱ�����Ѿ�����������
				width,
				height,
				1,						//minimap����1
				bIsRenderable ? D3DUSAGE_RENDERTARGET : 0,	//������D3DUSAGE_DYNAMIC ����Ҫ�Կ�֧�֣�D3DUSAGE_DYNAMIC˵������lock��������D3DPOOL_DEFAULT˵���������agp�д���
				D3DTypeConverter::toD3DFormat(texPixelFormat),		//���ǵĸ�ʽ
				bIsRenderable ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED,
				&d3dTexture,
				NULL))){
				return false;
			}

			if (bIsRenderable){
				D3DRenderWindow* curWindow = (D3DRenderWindow*)static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getCurRenderWindow();
				D3DPRESENT_PARAMETERS*	curWindowD3Dpp = curWindow->getPresentationParameters();
				const D3DMULTISAMPLE_TYPE MultiSample = D3DMULTISAMPLE_NONE;
				const DWORD MultisampleQuality = 0;
				const bool Discard = TRUE;

				if (FAILED(pD3DDevice->CreateDepthStencilSurface(
					width,
					height,
					curWindowD3Dpp->AutoDepthStencilFormat,
					MultiSample,
					MultisampleQuality,
					Discard,
					&pDepthStencilSurface,
					NULL
					))){
						return false;
				}				
			}

			 
		}else{
			Image img;
			assert(!fileName.empty());
			img.loadFromFile(fileName);

			u32 srcWidth = img.getWidth();
			u32 srcHeight= img.getHeight();

			u32 PO2Width, PO2Height;
			if (!MathUtil::isPO2(srcWidth) || !MathUtil::isPO2(srcHeight)) {

				PO2Width  = MathUtil::firstPO2From(srcWidth);
				PO2Height = MathUtil::firstPO2From(srcHeight);
				if (PO2Width == 0)
					PO2Width = 1;	

				if (PO2Height == 0)
					PO2Height = 1;

			}else {
				PO2Width  = srcWidth;
				PO2Height = srcHeight;
			}

			width = PO2Width;
			height= PO2Height;
				
			if (FAILED(pD3DDevice->CreateTexture( //�˺����������width��heightΪ2���ݴη����򴴽�ʧ��
				width,
				height,
				1,						//minimap����1
				0,
				D3DFMT_A8R8G8B8,		//���ǵĸ�ʽ
				D3DPOOL_MANAGED,        //��ϵͳ�����ڴ� �豸��ʧʱ�����ؽ�
				&d3dTexture,
				NULL))){
				return false;
			}

			D3DLOCKED_RECT d3dlr;//D3DPOOL_MANAGEDϵͳ�ڴ�ؿ���lock

			d3dTexture->LockRect(0, &d3dlr, 0, 0);
		
			u8 *pDestBuf = (u8*)d3dlr.pBits;

			u8 *pSrcBuf  = (u8*)img.getImageData();
			//�Ժ�Ϊimage��Ӻ������Դ�һ�ָ�ʽ��ͼƬת��Ϊ��һ�ָ�ʽ��ͼƬ���� rgb תΪ argb
			int PixelFB = GraphicsUtil::GetPixelFormatBytes(img.getPixelFormat());
			assert(PixelFB == 4);

			//memcpy(pDestBuf, img.getImageData(), width * height * 4);
			
			for (u32 y = 0; y < srcHeight; ++y){
				memcpy(pDestBuf + y * PO2Width * PixelFB, pSrcBuf + y * srcWidth * PixelFB, srcWidth * PixelFB);
			}

			d3dTexture->UnlockRect(0);

		}

		return true;
	}
}













