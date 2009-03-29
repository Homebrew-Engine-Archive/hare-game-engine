#include "PCH.h"
#include "SimpleSprite.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "RenderSystem.h"
#include "Canvas.h"

namespace hare_graphics
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

		//RotatorMod* rotatorMod;
		//rotatorMod = new RotatorMod;
		//rotatorMod->center.x = 0.5f;
		//rotatorMod->center.y = 0.5f;
		//rotatorMod->rotation = 10.5f;
		//rotatorMod->speed = 10.5f;
		//rotatorMod->setSubMtrl(scaleMtrl);

		AnimModUnit* animModUnit;
		animModUnit = new AnimModUnit;
		animModUnit->frameTime = 1;
		//animModUnit->setSubMtrl(pannerMtrl);
		animModUnit->setSubMtrl(scaleMtrl);
		//animModUnit->setSubMtrl(rotatorMod);

		AnimMod* animModMtrl = new AnimMod;
		animModMtrl->addFrame(animModUnit);

		shader = new SimpleShader;
		shader->setMaterial(animModMtrl);
		quad.normalize();
		quad.setWidth((float)tex->getWidth());
		quad.setHeight((float)tex->getHeight());
		quad.setMaterial(shader);
		quad.moveTo(100, 100);
	}

	void SimpleSprite::beginScene()
	{
		shader->getMaterial()->frameMove();
	}

	void SimpleSprite::renderScene()
	{
		RenderSystem::getSingletonPtr()->render(&quad);
	}

	void SimpleSprite::endScene()
	{

	}

	void SimpleSprite::move(int x, int y)
	{
		quad.move((f32)x, (f32)y);
	}
}
