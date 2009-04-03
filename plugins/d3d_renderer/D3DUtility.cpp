#include "PCH.h"
#include "D3DUtility.h"
#include "D3DTypeConverter.h"
#include "D3DRenderSystem.h"

namespace hare_d3d
{
	void D3DUtility::downLoadSurface(LPDIRECT3DSURFACE9 d3dSur, HarePixelFormat format, Image& desImg, const RectN& rc)
	{
		assert(d3dSur);

		HRESULT hr;
		D3DSURFACE_DESC desc;
		d3dSur->GetDesc(&desc);

		assert(desc.Width >= (u32)rc.width() && desc.Height >= (u32)rc.height());
		assert(D3DTypeConverter::toD3DFormat(format) == desc.Format);

		RectN rect;
		if (rc.width() == 0 || rc.height() == 0){
			rect.minX = 0;
			rect.maxX = desc.Width;
			rect.minY = 0;
			rect.maxY = desc.Height;
		}else{
			rect = rc;
		}

		desImg.create(rect.width(), rect.height(), format);

		if (desc.Usage == D3DUSAGE_RENDERTARGET){
			LPDIRECT3DDEVICE9 pD3DDevice = static_cast<D3DRenderSystem*>(RenderSystem::getSingletonPtr())->getD3DDevice();
			IDirect3DTexture9* tmpD3dTex;	
			hr = pD3DDevice->CreateTexture(
				rect.width(),
				rect.height(),
				0,
				D3DUSAGE_DYNAMIC,
				desc.Format,
				D3DPOOL_SYSTEMMEM,
				&tmpD3dTex,
				NULL
				);
			LPDIRECT3DSURFACE9 destSurface;
			hr = tmpD3dTex->GetSurfaceLevel(0,&destSurface);
			hr = pD3DDevice->GetRenderTargetData(d3dSur,destSurface);

			D3DLOCKED_RECT lockedRect;

			//上下翻转
			RECT winrc = { rect.minX, desc.Height - rect.maxY, rect.maxX, desc.Height - rect.minY };
			hr = destSurface->LockRect(&lockedRect, &winrc, D3DLOCK_READONLY);

			int desRowStride = desImg.getRowStride();
			u8* desData = (u8*)desImg.getImageData();

			const u8* srcData = (const u8*)lockedRect.pBits + (desImg.getHeight() - 1) * lockedRect.Pitch;
			for (u32 i = 0; i < desImg.getHeight(); ++i) {
				memcpy(desData, srcData, desRowStride);

				desData += desRowStride;
				srcData -= lockedRect.Pitch;
			}

			destSurface->UnlockRect();

			SAFE_RELEASE(destSurface);
			SAFE_RELEASE(tmpD3dTex);	

		}else{
			D3DLOCKED_RECT lockedRect;

			//上下翻转
			RECT winrc = { rect.minX, desc.Height - rect.maxY, rect.maxX, desc.Height - rect.minY };
			hr = d3dSur->LockRect(&lockedRect, &winrc, D3DLOCK_READONLY);

			int desRowStride = desImg.getRowStride();
			u8* desData = (u8*)desImg.getImageData();

			const u8* srcData = (const u8*)lockedRect.pBits + (desImg.getHeight() - 1) * lockedRect.Pitch;
			for (u32 i = 0; i < desImg.getHeight(); ++i) {
				memcpy(desData, srcData, desRowStride);

				desData += desRowStride;
				srcData -= lockedRect.Pitch;
			}

			d3dSur->UnlockRect();
		}
	}
}