#include "PCH.h"
#include "GLRenderSystem.h"
#include "GLTypeConverter.h"
#include "GLTexture.h"
#include "GLVertexBufferManager.h"

GLRenderSystem::GLRenderSystem()
    :curRenderTexture(0)
    ,PrimType(GL_QUADS)
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
	if (GLVertexBufferManager::getSingletonPtr()->getArrayCount() > 0){
        GLVertexBufferManager::getSingletonPtr()->lock();
		glDrawArrays(PrimType, 0, GLVertexBufferManager::getSingletonPtr()->getArrayCount());
        GLVertexBufferManager::getSingletonPtr()->unlock();
	}
}

void GLRenderSystem::render(RenderUnit* operation)
{
    if (!operation)
        return ;

	bool bShaderParamsChange = false; 
	bool bTextureStageChange = false; 
	bool bRenderTextureChange= false; 
	Matrix4 tmpTexMat;



	Material* mtrl = operation->getMaterial();
	if (mtrl){
		Shader* shader = mtrl->getShader();
		ShaderParams tmpShaderParams;
		if (shader){
			tmpShaderParams = shader->getShaderParams();
		}

		if (curShaderParams != tmpShaderParams){
			curShaderParams = tmpShaderParams;
			bShaderParamsChange = true;
		}

		TextureMtrl* textureMtrl = mtrl->getTextureMtrl();
		if (textureMtrl){
			if (curTextureStage != textureMtrl->getTextureStage()){
				curTextureStage = textureMtrl->getTextureStage();
				bTextureStageChange = true;
			}

			tmpTexMat = textureMtrl->texMat;

			GLTexture* texture = (GLTexture*)textureMtrl->getTexture();
			if (texture){
				if (curRenderTexture != texture->getGLTexture()){
					bRenderTextureChange = true;
					curRenderTexture = texture->getGLTexture();
				}
			}else{
				assert(false);
			}

		}else{
			bRenderTextureChange = true;
			curRenderTexture = 0;
			tmpTexMat = Matrix4::IDENTITY;
		}
	}else{
		bRenderTextureChange = true;
		curRenderTexture = 0;	
		tmpTexMat = Matrix4::IDENTITY;	
	}

	if ( bRenderTextureChange || bTextureStageChange || bShaderParamsChange
	    || PrimType != GLTypeConverter::toGLPrimtiveType(operation->getOperationType())
        || GLVertexBufferManager::getSingletonPtr()->getArrayCount() >= VERTEX_BUFFER_COUNT - operation->getVertexCount()
		|| texMat != tmpTexMat){

        render();

		PrimType = GLTypeConverter::toGLPrimtiveType(operation->getOperationType());
		
		if (bRenderTextureChange){
			if (curRenderTexture){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, curRenderTexture);
			}else{
				glDisable(GL_TEXTURE_2D);
			}
		}

		if (bShaderParamsChange)
			setShaderParams(curShaderParams);

		if (bTextureStageChange)
			setTextureStage(curTextureStage);

		texMat = tmpTexMat;
		makeGLMatrix(gl_matrix, texMat);
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(gl_matrix);


	}

	GLVertexBufferManager::getSingletonPtr()->writeBuffer(operation->getBuffer(), operation->getVertexCount());
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
			,GLTypeConverter::toGLSceneBlendArg((ShaderParams::SceneBlendArgument)shaderParams.SceneBlendDesArg));
	}else{
		glDisable(GL_BLEND);
	}

	RenderWindow* w = getCurRenderWindow();

	if (w){	
		if (w->getWindowParams().bZbuffer){
			if (shaderParams.bUseZ){
                glEnable(GL_DEPTH_TEST);
			}else{
                glDisable(GL_DEPTH_TEST);			
			}
		}
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

void GLRenderSystem::initalizeParam(bool bZBuffer)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	if (bZBuffer){
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);	
	}
	setShaderParams(curShaderParams);
	setTextureStage(curTextureStage);
}

GLuint GLRenderSystem::getCurTexture()
{
	return curRenderTexture;
}

void GLRenderSystem::setProjection(float l, float r, float b, float t)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //NB: glOrtho funcation last tow args is used as negative
    //left right bottom top                                                 near far
    glOrtho(l, r, b, t, -1.0, 1.0); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

    glViewport (0, 0, (GLsizei)(r - l), (GLsizei)(t - b));
}

void GLRenderSystem::prepareCanvasRender()
{
    glEnable(GL_TEXTURE_2D);

    int l = 0;
    int r = RenderSystem::getSingletonPtr()->getCurRenderWindow()->getWindowParams().width;
    int b = RenderSystem::getSingletonPtr()->getCurRenderWindow()->getWindowParams().height;
    int t = 0;

    setProjection((float)l, (float)r, (float)b, (float)t);
}