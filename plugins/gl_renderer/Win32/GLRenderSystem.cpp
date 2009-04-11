#include "PCH.h"
#include "GLRenderSystem.h"

GLRenderSystem::GLRenderSystem()
{

}

GLRenderSystem::~GLRenderSystem()
{

}

void GLRenderSystem::initalize()
{

}

void GLRenderSystem::release()
{

}

void GLRenderSystem::beginFrame()
{

}

void GLRenderSystem::render()
{

}

void GLRenderSystem::render(RenderUnit* operation)
{
    if (!operation)
        return ;

    ShaderParams tmpShaderParams;
    TextureStage tmpTextureStage;
    GLTexture*   texture   = NULL;
    Matrix4      tmpTexMat = Matrix4::IDENTITY;

    Material* mtrl = operation->getMaterial();
    if (mtrl){
        Shader* shader = mtrl->getShader();

        if (shader){
            tmpShaderParams = shader->getShaderParams();
        }

        TextureMtrl* textureMtrl = mtrl->getTextureMtrl();
        if (textureMtrl){

            tmpTexMat = textureMtrl->texMat;

            texture = (D3DTexture*)textureMtrl->getTexture();
            if (!texture){
               assert(false);
            }
        }
    }

    setShaderParams(tmpShaderParams);

    setTextureStage(tmpTextureStage);

    if (texture){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture->getGLTexture());
    }else{
        glDisable(GL_TEXTURE_2D);
    }



}

void GLRenderSystem::endFrame()
{

}

void GLRenderSystem::clear(bool z)
{

}

void GLRenderSystem::setShaderParams(const ShaderParams& shaderParams)
{

}

void GLRenderSystem::setTextureStage(const TextureStage& textureStage)
{

}

Texture* GLRenderSystem::createTexture()
{
    return new GLTexture;
}