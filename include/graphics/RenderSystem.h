#ifndef RENDERSYSTEM
#define RENDERSYSTEM


#include "GraphicsPrerequisites.h"
#include "Material.h"
#include "Shader.h"

namespace hare_graphics
{
	class RenderTarget;
	class RenderWindow;
	class Texture;
	class RenderOperation;

	class GRAPHICS_API RenderSystem : public Singleton<RenderSystem>
	{
		HARE_DECLARE_SINGLETON(RenderSystem)
	public:
		RenderSystem();
		virtual ~RenderSystem();

		virtual void resetDevice() = 0;
		virtual void initalize() = 0;
		virtual void release() = 0;

		virtual void beginFrame() = 0;
		virtual void render() = 0;
		virtual void render(RenderOperation* operation) = 0;
		virtual void endFrame() = 0;
		virtual void clear() = 0;

		virtual void setShaderParams(const ShaderParams& shaderParams) = 0;
		virtual void setTextureStage(const TextureStage& textureStage) = 0;

		void setRenderTarget(RenderTarget* target);
		void setCurRenderWindow(RenderWindow* window);
		RenderTarget* getRenderTarget();
		RenderWindow* getCurRenderWindow();

		virtual Texture* createTexture() = 0;

	protected:
		RenderTarget* curRenderTarget;
		RenderWindow* curRenderWindow;

	};
}

#endif