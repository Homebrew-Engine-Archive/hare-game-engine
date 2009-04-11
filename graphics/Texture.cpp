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


	void Texture::create(uint32 w, uint32 h, HarePixelFormat format, bool isRenderable)
	{
		size = 0;
		width = w;	//转换成2的幂次方
		height = h;	//转换成2的幂次方
		projectionWidth = (float)w;//默认设置投影宽度
		projectionHeight= (float)h;//默认设置投影高度
		fileName = "";
		texPixelFormat = format;//动态纹理像素格式

		//顺序不可以改变 
		release();//释放上一次的资源 bIsRenderable = true; 放在此句话的后面是确保正确释放上一次的资源
		bIsRenderable = isRenderable; //如果不是renderable则使用系统托管的内存
		bFromImage    = false;  
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
		bFromImage    = true; 
		reCreate();
	}

	void Texture::setProjectionSize(float w, float h)
	{
		projectionWidth = w;
		projectionHeight= h;
	}
}
