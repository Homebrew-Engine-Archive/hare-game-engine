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
		quad.setShader(NULL);
	}
	SimpleSprite::~SimpleSprite()
	{

	}

	void SimpleSprite::loadFromImage(const String& filename)
	{
		shader = new SimpleShader;
		AnimMod* animModMtrl = new AnimMod;
		TextureMtrl* texMtrl;
		//PannerMod*   pannerMtrl;
		//ScalerMod*   scaleMtrl;
		AnimModUnit* animModUnit;
		Texture* tex;



		//tex = RenderSystem::getSingletonPtr()->createTexture();
		//tex->createFromFile("haha.png");
		//texMtrl = new TextureMtrl;
		//texMtrl->setTexture(tex);
		//animModUnit = new AnimModUnit;
		//animModUnit->frameTime = 1;
		//animModUnit->setSubMtrl(texMtrl);
		//animModMtrl->addFrame(animModUnit);

		tex = RenderSystem::getSingletonPtr()->createTexture();
		tex->createFromFile(filename);
		texMtrl = new TextureMtrl;
		texMtrl->setTexture(tex);

		//pannerMtrl = new PannerMod;
		//pannerMtrl->direction.x = 0.1f;
		//pannerMtrl->direction.y = 0.1f;
		//pannerMtrl->speed = 1.f;
		//pannerMtrl->setSubMtrl(texMtrl);

		//scaleMtrl = new ScalerMod;
		//scaleMtrl->scale.y = 0.5f;
		//scaleMtrl->setSubMtrl(pannerMtrl);

		animModUnit = new AnimModUnit;
		animModUnit->frameTime = 1;
		animModUnit->setSubMtrl(texMtrl);
		animModMtrl->addFrame(animModUnit);

		shader->setMaterial(animModMtrl);
		quad.normalize();
		quad.setWidth((float)tex->getWidth());
		quad.setHeight((float)tex->getHeight());
		quad.setShader(shader);
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