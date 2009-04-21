#include "PCH.h"
#include "SimpleSprite.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "RenderSystem.h"
#include "Canvas.h"
#include "SimpleParticle.h"

namespace hare
{
	SimpleSprite::SimpleSprite()
		:shader(NULL)
	{
		quad.setMaterial(NULL);
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

		AnimModUnit* animModUnit;
		animModUnit = new AnimModUnit;
		animModUnit->frameTime = 1;
		//animModUnit->setSubMtrl(texMtrl);
		//animModUnit->setSubMtrl(pannerMtrl);
		//animModUnit->setSubMtrl(scaleMtrl);
		animModUnit->setSubMtrl(rotatorMod);

		AnimMod* animModMtrl = new AnimMod;
		animModMtrl->addFrame(animModUnit);

		shader = new SimpleShader;
		shader->setMaterial(animModMtrl);
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
        //particle->saveToXml("default.particle");

	}

	void SimpleSprite::beginScene()
	{
		shader->getMaterial()->frameMove();
		particle->frameMove();
	}

	void SimpleSprite::renderScene()
	{
		RenderTarget* target = RenderSystem::getSingletonPtr()->getRenderTarget();

		RenderSystem::getSingletonPtr()->render(&quad);
		particle->render();

		Texture* tex;
		tex = RenderSystem::getSingletonPtr()->createTexture();
		tex->create(256, 256, HPF_A8R8G8B8, true);
		RenderSystem::getSingletonPtr()->setRenderTarget(tex);

		RenderSystem::getSingletonPtr()->render(&quad);

		RenderSystem::getSingletonPtr()->setRenderTarget(target);

		TextureMtrl* texMtrl;
		texMtrl = new TextureMtrl;
		texMtrl->setTexture(tex);

		getCanvas()->drawImage(300, 300, texMtrl);

	}

	void SimpleSprite::endScene()
	{

	}

	void SimpleSprite::move(float x, float y)
	{
		quad.move(x, y);
		particle->move(x, y);
	}
}
