//***************************************************************
//  File:    Texture.cpp
//  Date:    09/30/2008
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "Texture.h"

namespace hare
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
		width = w;	//ת����2���ݴη�
		height = h;	//ת����2���ݴη�
		projectionWidth = (float)w;//Ĭ������ͶӰ���
		projectionHeight= (float)h;//Ĭ������ͶӰ�߶�
		fileName = "";
		texPixelFormat = format;//��̬�������ظ�ʽ

		//˳�򲻿��Ըı�
		release();//�ͷ���һ�ε���Դ bIsRenderable = true; ���ڴ˾仰�ĺ�����ȷ����ȷ�ͷ���һ�ε���Դ
		bIsRenderable = isRenderable; //�������renderable��ʹ��ϵͳ�йܵ��ڴ�
		bFromImage    = false;
		recreate();
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
		bFromImage    = true;
		recreate();
	}

	void Texture::setProjectionSize(float w, float h)
	{
		projectionWidth = w;
		projectionHeight= h;
	}
}
