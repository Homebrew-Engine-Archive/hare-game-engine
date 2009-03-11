#include "PCH.h"
#include "Texture.h"

namespace hare_graphics
{
	HARE_IMPLEMENT_ABSTRACT_CLASS(Texture, Object, 0)
	{}

	Texture::Texture(void)
		:bIsRenderable(false)
	{
	}

	Texture::~Texture(void)
	{
	}


	void Texture::createRenderToTex(u32 w, u32 h)
	{
		size = 0;
		width = w;	//转换成2的幂次方
		height = h;	//转换成2的幂次方
		projectionWidth = (f32)w;//默认设置投影宽度
		projectionHeight= (f32)h;//默认设置投影高度
		fileName = "";

		//顺序不可以改变 
		release();//释放上一次的资源 bIsRenderable = true; 放在此句话的后面是确保正确释放上一次的资源
		bIsRenderable = true;
		reCreate();
	}

	void Texture::createFromFile(const String& filename)
	{
		assert(!filename.empty());
		size = 0;
		width = 0;
		height = 0;
		fileName = filename;

		//顺序不可以改变 
		release();//释放上一次的资源 bIsRenderable = false; 放在此句话的后面是确保正确释放上一次的资源
		bIsRenderable = false;
		reCreate();
	}

	void Texture::setProjectionSize(f32 w, f32 h)
	{
		projectionWidth = w;
		projectionHeight= h;
	}
}
