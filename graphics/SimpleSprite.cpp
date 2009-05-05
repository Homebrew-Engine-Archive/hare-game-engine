#include "PCH.h"
#include "SimpleSprite.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "RenderSystem.h"
#include "Canvas.h"
#include "SimpleParticle.h"
#include "Animation.h"


namespace hare
{

	Animation* anim;

	SimpleSprite::SimpleSprite()
		:shader(NULL)
	{
		quad.setMaterial(NULL);
		anim = new Animation;


		_tex = RenderSystem::getSingletonPtr()->createTexture();
		_tex->create(256, 256, HPF_A8R8G8B8, true);

		_texMtrl = new TextureMtrl;
		_texMtrl->setTexture(_tex);

	}
	SimpleSprite::~SimpleSprite()
	{
        delete anim;
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

		AnimFrame* frame = new AnimFrame;
		frame->frameTime = 1;
		frame->setMaterial(shader);
        anim->addFrame(frame);

		frame = new AnimFrame;
		frame->frameTime = 2;

		tex = RenderSystem::getSingletonPtr()->createTexture();
		tex->createFromFile("/sample/images/sample.png");
		texMtrl = new TextureMtrl;
		texMtrl->setTexture(tex);

		frame->setMaterial(texMtrl);
        anim->addFrame(frame);

		anim->moveTo(300, 500);

	}

	void SimpleSprite::beginScene()
	{
		
	}

	void SimpleSprite::renderScene()
	{
		//simple test
		RenderTarget* target = RenderSystem::getSingletonPtr()->getRenderTarget();

        shader->getMaterial()->frameMove();
		RenderSystem::getSingletonPtr()->render(&quad);

		particle->render();

		anim->render();

		RenderSystem::getSingletonPtr()->setRenderTarget(_tex);

		shader->getMaterial()->frameMove();
		RenderSystem::getSingletonPtr()->render(&quad);

		RenderSystem::getSingletonPtr()->setRenderTarget(target);

        //getCanvas()->drawImage(300, 300, shader, RectF(0.5,0.5,1.0,1.0));
        getCanvas()->drawImage(600, 300, _texMtrl);

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
