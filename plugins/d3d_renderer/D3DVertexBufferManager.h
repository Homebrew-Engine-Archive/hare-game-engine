#ifndef D3DVECTEXBUFFERMANAGER
#define D3DVECTEXBUFFERMANAGER

#include "DeviceManager.h"

namespace hare_d3d
{
	class D3DVertexBufferManager : public Singleton<D3DVertexBufferManager> , public DeviceObject 
	{
		HARE_DECLARE_SINGLETON(D3DVertexBufferManager)

		friend class D3DRenderSystem;
	public:

		D3DVertexBufferManager(u32 size, u32 FVF);
		virtual ~D3DVertexBufferManager();

		virtual void beforeResetDevice();
		virtual void afterResetDevice();

		void lockBuffer();
		void unlockBuffer();
		void writeBuffer(Vertex* buffer, u32 vertexCnt, u32 primCnt);
		//void readBuffer(Vertex* buffer, u32 size);

		u32 getVectexCount();
		u32 getPrimCount();

		void release();

	private:
		Vertex* vertexBuffer;

		bool isLock;

		LPDIRECT3DVERTEXBUFFER9 d3dVectexBuffer;

		u32 vertexCount;

		u32 primCount;

		const u32 bufferSize;

		const u32 vertexFVF;
	};
}

#endif