#include "PCH.h"
#include "D3DIndexBufferManager.h"
#include "D3DRenderSystem.h"




namespace hare_d3d
{

	HARE_IMPLEMENT_SINGLETON(D3DIndexBufferManager)

	D3DIndexBufferManager::D3DIndexBufferManager(u32 size)
		:IndexBufferSize(size)
		,d3dIndexBuffer(NULL)
	{
		afterResetDevice();
	}

	D3DIndexBufferManager::~D3DIndexBufferManager()
	{
		release();
	}

	void D3DIndexBufferManager::beforeResetDevice()
	{
		release();
	}

	void D3DIndexBufferManager::afterResetDevice()
	{
		if (d3dIndexBuffer){
			return ;
		}

		LPDIRECT3DDEVICE9 d3dDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
		assert(d3dDevice);

		if (FAILED(d3dDevice->CreateIndexBuffer(
			IndexBufferSize * 3, // 
			D3DUSAGE_WRITEONLY, 
			D3DFMT_INDEX16, 
			D3DPOOL_DEFAULT, 
			&d3dIndexBuffer,
			NULL))){
			assert(false);
		}

		WORD *pIndices, n = 0;

		if (FAILED(d3dIndexBuffer->Lock(0, 0, (void**)&pIndices, 0))){
			assert(false);
		}

		for(u32 i = 0; i < IndexBufferSize / 4; i++)
		{
			*pIndices++ = n;
			*pIndices++ = n + 1;
			*pIndices++ = n + 2;
			*pIndices++ = n + 2;
			*pIndices++ = n + 3;
			*pIndices++ = n;
			n += 4;
		}

		d3dIndexBuffer->Unlock();
		d3dDevice->SetIndices(d3dIndexBuffer);

	}

	void D3DIndexBufferManager::release()
	{
		SAFE_RELEASE(d3dIndexBuffer);
		LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
		pD3DDevice->SetIndices(NULL);
	}
}