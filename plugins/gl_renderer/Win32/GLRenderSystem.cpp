#include "PCH.h"
#include "GLRenderSystem.h"
#include "GLTypeConverter.h"
#include "GLTexture.h"

GLRenderSystem::GLRenderSystem()
{

}

GLRenderSystem::~GLRenderSystem()
{

}

void GLRenderSystem::initalize()
{
    glEnable(GL_TEXTURE_2D);
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

void GLRenderSystem::writeBuffer(GLenum type, Vertex* buffer, uint32 count)
{
    Color   color;
    uint32  numPerUnit = GLTypeConverter::countByPrimtType(type);

    glBegin(type);
    for (uint32 i = 0; i < count; i += numPerUnit){
        for (uint32 j = 0; j < numPerUnit; ++j){
            color = buffer[i + j].diffuse;
            glColor4f(color.R, color.G, color.B, color.A);
            glTexCoord2f(buffer[i + j].u, buffer[i + j].v);
            glVertex3f(buffer[i + j].x, buffer[i + j].y, buffer[i + j].z);
        }
    }
    glEnd();
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

    if (texture){
        glBindTexture(GL_TEXTURE_2D, texture->getGLTexture());

        setShaderParams(tmpShaderParams);

        setTextureStage(tmpTextureStage);
    }else{
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLfloat gl_matrix[16];
    makeGLMatrix(gl_matrix, tmpTexMat);

    glPushMatrix();
    glLoadMatrixf(gl_matrix);
    writeBuffer(GLTypeConverter::toGLPrimtiveType(operation->getOperationType(), operation->getBuffer(), operation->getVertexCount()));
    glPopMatrix();
}

void GLRenderSystem::endFrame()
{

}

void GLRenderSystem::clear(bool z)
{
    GLbitfield flag = GL_COLOR_BUFFER_BIT;
    glClearColor(0, 0, 0, 0);
    if (z){
        flag |= GL_DEPTH_BUFFER_BIT;
        glClearDepth(1.0f);
    }
    glClear(flag);
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

void GLRenderSystem::makeGLMatrix(GLfloat gl_matrix[16], const Matrix4& m)
{
    size_t x = 0;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            gl_matrix[x] = m[j][i];
            x++;
        }
    }
}