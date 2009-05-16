//***************************************************************
//  File:    SimpleSprite.cpp
//  Date:    5/10/2009
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
#include "SimpleSprite.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "RenderSystem.h"
#include "Canvas.h"
#include "SimpleParticle.h"
#include "AnimationSprite.h"


namespace hare
{



	SimpleSprite::SimpleSprite()
		:shader(NULL)
	{
		quad.setMaterial(NULL);


		//_tex = RenderSystem::getSingletonPtr()->createTexture();
		//_tex->create(256, 256, HPF_A8R8G8B8, true);

		//_texMtrl = new TextureMtrl;
		//_texMtrl->setTexture(_tex);

	}
	SimpleSprite::~SimpleSprite()
	{

	}

	void SimpleSprite::loadFromImage(const String& filename)
	{
		Texture* tex;
		tex = RenderSystem::getSingletonPtr()->createTexture();
		tex->createFromFile(filename);

		TextureMtrl* texMtrl;
		texMtrl = new TextureMtrl;
		texMtrl->setTexture(tex);

		//PannerMod*   pannerMtrl;
		//pannerMtrl = new PannerMod;
		//pannerMtrl->offset.x = 0.5f;
		//pannerMtrl->panDirection.x = 0.5f;
		//pannerMtrl->panRate = 0.5f;
		//pannerMtrl->setSubMtrl(texMtrl);
		//
		ScalerMod*   scaleMtrl;
		scaleMtrl = new ScalerMod;
		scaleMtrl->scale.x = 0.5f;
		scaleMtrl->scale.y = 0.5f;
		//scaleMtrl->center.x = 0.2f;
		//scaleMtrl->center.x = 0.2f;
		scaleMtrl->setSubMtrl(texMtrl);

		RotatorMod* rotatorMod;
		rotatorMod = new RotatorMod;
		//rotatorMod->center.x = 0.5f;
		//rotatorMod->center.y = 0.5f;
		rotatorMod->rotation = 10.5f;
		rotatorMod->speed = 30.5f;
		rotatorMod->setSubMtrl(scaleMtrl);

		shader = new SimpleShader;
		shader->setMaterial(rotatorMod);
		quad.normalize();
		quad.setWidth((float)tex->getWidth());
		quad.setHeight((float)tex->getHeight());
		quad.setMaterial(shader);
		quad.moveTo(0, 0);


		tex = RenderSystem::getSingletonPtr()->createTexture();
		tex->createFromFile(filename);
		texMtrl = new TextureMtrl;
		texMtrl->setTexture(tex);
		ParticleShader* particleMtrl = new ParticleShader;
		particleMtrl->setMaterial(texMtrl);
		particle = new SimpleParticle;
		particle->setMaterial(particleMtrl);
		particle->fireAt(200, 200);

	}

	void SimpleSprite::render()
	{
		//simple test
		RenderTarget* target = RenderSystem::getSingletonPtr()->getRenderTarget();

        shader->getMaterial()->frameMove();
		RenderSystem::getSingletonPtr()->render(&quad);

		particle->render();

		//RenderSystem::getSingletonPtr()->setRenderTarget(_tex);

		//shader->getMaterial()->frameMove();
		//RenderSystem::getSingletonPtr()->render(&quad);

		//RenderSystem::getSingletonPtr()->setRenderTarget(target);

  //      //getCanvas()->drawImage(300, 300, shader, RectF(0.5,0.5,1.0,1.0));
  //      getCanvas()->drawImage(600, 300, _texMtrl);

	}

	void SimpleSprite::move(float dx, float dy)
	{
		quad.move(dx, dy);
		particle->move(dx, dy);
	}

    void SimpleSprite::moveTo(float x, float y)
    {
        quad.moveTo(x, y);
        particle->moveTo(x, y);
    }
}


