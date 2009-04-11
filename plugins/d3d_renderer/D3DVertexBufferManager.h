#ifndef _D3DVECTEXBUFFERMANAGER_H_
#define _D3DVECTEXBUFFERMANAGER_H_

#include "DeviceManager.h"


class D3DVertexBufferManager : public Singleton<D3DVertexBufferManager> , public DeviceObject 
{
	HARE_DECLARE_SINGLETON(D3DVertexBufferManager)

	friend class D3DRenderSystem;
public:

	D3DVertexBufferManager(uint32 size, uint32 FVF);
	virtual ~D3DVertexBufferManager();

	virtual void beforeResetDevice();
	virtual void afterResetDevice();

	void lockBuffer();
	void unlockBuffer();
	void writeBuffer(Vertex* buffer, uint32 vertexCnt, uint32 primCnt);
	//void readBuffer(Vertex* buffer, uint32 size);

	uint32 getVectexCount();
	uint32 getPrimCount();

	void release();

private:
	Vertex* vertexBuffer;

	bool isLock;

	LPDIRECT3DVERTEXBUFFER9 d3dVectexBuffer;

	uint32 vertexCount;

	uint32 primCount;

	const uint32 bufferSize;

	const uint32 vertexFVF;
};


#endif