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
	//if (PrimType == GL_LINES)
	//	glDisable(GL_TEXTURE_2D);

	if (GLVertexBufferManager::getSingletonPtr()->getArrayCount() > 0){
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		//glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLVertexBufferManager::getSingletonPtr()->getVertexVBO());
		//glVertexPointer(3, GL_FLOAT, 0, NULL);		
		//glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLVertexBufferManager::getSingletonPtr()->getTexCoordVBO());
		//glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		//glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLVertexBufferManager::getSingletonPtr()->getColorVBO());
		//glColorPointer(4, GL_UNSIGNED_BYTE, 0, NULL);


		glVertexPointer(3, GL_FLOAT, 0, GLVertexBufferManager::getSingletonPtr()->getVertexArray());		
		glTexCoordPointer(2, GL_FLOAT, 0, GLVertexBufferManager::getSingletonPtr()->getTexCoordArray());
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, GLVertexBufferManager::getSingletonPtr()->getColorArray());



		glDrawArrays(PrimType, 0, GLVertexBufferManager::getSingletonPtr()->getArrayCount());

		glDisableClientState(GL_VERTEX_ARRAY);				
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);	

        GLVertexBufferManager::getSingletonPtr()->arrayCount = 0;
	}
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

	//if (!operation)
	//	return ;

	//ShaderParams tmpShaderParams;
	//TextureStage tmpTextureStage;
	//GLTexture*   texture   = NULL;
	//Matrix4      tmpTexMat = Matrix4::IDENTITY;

	//Material* mtrl = operation->getMaterial();
	//if (mtrl){
	//	Shader* shader = mtrl->getShader();

	//	if (shader){
	//		tmpShaderParams = shader->getShaderParams();
	//	}

	//	TextureMtrl* textureMtrl = mtrl->getTextureMtrl();
	//	if (textureMtrl){

	//		tmpTexMat = textureMtrl->texMat;

	//		texture = (GLTexture*)textureMtrl->getTexture();
	//		if (!texture){
	//			assert(false);
	//		}
	//	}
	//}

	//if (texture){
	//	glEnable(GL_TEXTURE_2D);

	//	glBindTexture(GL_TEXTURE_2D, texture->getGLTexture());

	//	setShaderParams(tmpShaderParams);

	//	setTextureStage(tmpTextureStage);
	//}else{
	//	glDisable(GL_TEXTURE_2D);
	//}

	//GLfloat gl_matrix[16];
	//makeGLMatrix(gl_matrix, tmpTexMat);

	//glMatrixMode(GL_TEXTURE);
	//glLoadMatrixf(gl_matrix);

	GLVertexBufferManager::getSingletonPtr()->writeBuffer(operation->getBuffer(), operation->getVertexCount());

	//PrimType = GLTypeConverter::toGLPrimtiveType(operation->getOperationType());

    //render();

	//writeBuffer(GLTypeConverter::toGLPrimtiveType(operation->getOperationType()), operation->getBuffer(), operation->getVertexCount());
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

void GLRenderSystem::initalizeParam()
{
	glewInit();
	GLenum ret = glGetError();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	setShaderParams(curShaderParams);
	setTextureStage(curTextureStage);
}