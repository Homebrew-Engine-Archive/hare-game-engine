#ifndef _D3DINDEXBUFFERMANAGER_H_
#define _D3DINDEXBUFFERMANAGER_H_

#include "DeviceManager.h"


class D3DIndexBufferManager : public Singleton<D3DIndexBufferManager> , public DeviceObject 
{
	HARE_DECLARE_SINGLETON(D3DIndexBufferManager)
public:
	D3DIndexBufferManager(uint32 size);
	virtual ~D3DIndexBufferManager();

	virtual void beforeResetDevice();
	virtual void afterResetDevice();

	void release();
private:
	const uint32 IndexBufferSize;

	LPDIRECT3DINDEXBUFFER9 d3dIndexBuffer;
};


#endif