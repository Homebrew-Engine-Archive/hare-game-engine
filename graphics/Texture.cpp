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
		width = w;	//ת����2���ݴη�
		height = h;	//ת����2���ݴη�
		projectionWidth = (f32)w;//Ĭ������ͶӰ���
		projectionHeight= (f32)h;//Ĭ������ͶӰ�߶�
		fileName = "";

		//˳�򲻿��Ըı� 
		release();//�ͷ���һ�ε���Դ bIsRenderable = true; ���ڴ˾仰�ĺ�����ȷ����ȷ�ͷ���һ�ε���Դ
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

		//˳�򲻿��Ըı� 
		release();//�ͷ���һ�ε���Դ bIsRenderable = false; ���ڴ˾仰�ĺ�����ȷ����ȷ�ͷ���һ�ε���Դ
		bIsRenderable = false;
		reCreate();
	}

	void Texture::setProjectionSize(f32 w, f32 h)
	{
		projectionWidth = w;
		projectionHeight= h;
	}
}
