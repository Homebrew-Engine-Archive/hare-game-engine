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

}

void GLRenderSystem::release()
{

}

void GLRenderSystem::beginFrame()
{

}

void GLRenderSystem::render()
{
	//glDrawBuffer(GL_FRONT_AND_BACK);
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

            texture = (GLTexture*)textureMtrl->getTexture();
            if (!texture){
               assert(false);
            }
        }
    }

    if (texture){
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, texture->getGLTexture());

		setShaderParams(tmpShaderParams);

        setTextureStage(tmpTextureStage);
    }else{
		glDisable(GL_TEXTURE_2D);
    }

    GLfloat gl_matrix[16];
    makeGLMatrix(gl_matrix, tmpTexMat);

	glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(gl_matrix);
    writeBuffer(GLTypeConverter::toGLPrimtiveType(operation->getOperationType()), operation->getBuffer(), operation->getVertexCount());
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
	if(shaderParams.AlphaBlendEnable){
		glEnable(GL_BLEND);

		glBlendFunc(GLTypeConverter::toGLSceneBlendArg((ShaderParams::SceneBlendArgument)shaderParams.SceneBlendSrcArg)
			, GLTypeConverter::toGLSceneBlendArg((ShaderParams::SceneBlendArgument)shaderParams.SceneBlendDesArg));
	}else{
		glDisable(GL_BLEND);
	}

	if(shaderParams.AlphaTestEnable){
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, shaderParams.AlphaRef / 255.0f);
	}else{
		glDisable(GL_ALPHA_TEST);
	}
}

void GLRenderSystem::setTextureStage(const TextureStage& textureStage)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLTypeConverter::toGLTextureWrapMode((TextureStage::WrapMode)textureStage.wrapModeU));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLTypeConverter::toGLTextureWrapMode((TextureStage::WrapMode)textureStage.wrapModeV));

	if (textureStage.fliterType == TextureStage::FT_Point){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}else if (textureStage.fliterType == TextureStage::FT_Line){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

Texture* GLRenderSystem::createTexture()
{
    return new GLTexture;
}

void GLRenderSystem::makeGLMatrix(GLfloat gl_matrix[16], const Matrix4& mat)
{
    size_t x = 0;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            gl_matrix[x] = mat.m[j][i];
            x++;
        }
    }
}