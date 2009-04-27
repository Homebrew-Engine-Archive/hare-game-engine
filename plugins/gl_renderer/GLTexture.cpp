#include "PCH.h"
#include "GLTexture.h"
#include "GLTypeConverter.h"
#include "GLRenderSystem.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

GLTexture::GLTexture()
    :glTexture(0)
	,fbo(0)
    ,depthbuffer(0)
{

}

GLTexture::~GLTexture()
{
    release();
}

void GLTexture::active()
{
    assert(bIsRenderable);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

    GLRenderSystem::getSingletonPtr()->clear(GLRenderSystem::getSingletonPtr()->getCurRenderWindow()->getWindowParams().bZbuffer);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 

    //NB: glOrtho funcation last tow args is used as negative       near far                
    glOrtho(0, (GLfloat)projectionWidth,  0, (GLfloat)projectionHeight,-1.0, 1.0); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

    glViewport(0, 0, (GLsizei)projectionWidth, (GLsizei)projectionHeight);

}

void GLTexture::inactive()
{
    assert(bIsRenderable);

	//release bind
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void GLTexture::upload(const Image& img, uint32 destX, uint32 destY)
{
	assert(img.getPixelFormat() == texPixelFormat);

	int desWidth = std::min(img.getWidth(), width - destX);
	int desHeight= std::min(img.getHeight(), height - destY);

	glBindTexture(GL_TEXTURE_2D, glTexture);

	glTexSubImage2D(GL_TEXTURE_2D, 0, destX, destY, desWidth, desHeight, GLTypeConverter::toGLFormat(img.getPixelFormat()), GL_UNSIGNED_BYTE, img.getImageData());

#ifdef _DEBUG
	GLenum ret = glGetError();
	assert(ret == GL_NO_ERROR);
#endif
}

void GLTexture::download(Image& img, const RectN& rc)
{

}

bool GLTexture::doCreate()
{
    if (!bFromImage){
        if (GLTypeConverter::toGLFormat(texPixelFormat) == GL_NONE)
            return false;

        int PixelFB = GraphicsUtil::getPixelFormatBytes(texPixelFormat);

        uint8* data = new uint8[width * height * PixelFB];

        memset(data, 0, width * height * PixelFB);

        glGenTextures(1, &glTexture);

#ifdef _DEBUG
		GLenum ret = glGetError();
#endif

        glBindTexture(GL_TEXTURE_2D, glTexture);

		//default filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 
            0,
            GLTypeConverter::getGLInternalFormat(texPixelFormat), 
            width, 
            height, 
            0, 
            GLTypeConverter::toGLFormat(texPixelFormat), 
            GL_UNSIGNED_BYTE, 
            0);

		glBindTexture(GL_TEXTURE_2D, 0);
        delete [] data;

		if (bIsRenderable){
            glGenFramebuffersEXT(1, &fbo);
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

            glGenRenderbuffersEXT(1, &depthbuffer);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, glTexture, 0);
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

#ifdef _DEBUG
			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			assert(status == GL_FRAMEBUFFER_COMPLETE_EXT);
#endif

		}

    }else{
        Image img;
        assert(!fileName.empty());
        img.loadFromFile(fileName);

        uint32 srcWidth = img.getWidth();
        uint32 srcHeight= img.getHeight();

        uint32 PO2Width, PO2Height;
        if (!MathUtil::isPO2(srcWidth) || !MathUtil::isPO2(srcHeight)) {

            PO2Width  = MathUtil::firstPO2From(srcWidth);
            PO2Height = MathUtil::firstPO2From(srcHeight);
            if (PO2Width == 0)
                PO2Width = 1;	

            if (PO2Height == 0)
                PO2Height = 1;

        }else {
            PO2Width  = srcWidth;
            PO2Height = srcHeight;
        }

        width = PO2Width;
        height= PO2Height;

        //以后为image添加函数可以从一种格式的图片转换为另一种格式的图片例如 rgb 转为 argb
        int PixelFB = GraphicsUtil::getPixelFormatBytes(img.getPixelFormat());
        assert(PixelFB == 4);

        uint8 *pDestBuf = new uint8[width * height * PixelFB];

		memset(pDestBuf, 0, width * height * PixelFB);

        uint8 *pSrcBuf  = (uint8*)img.getImageData();

        //swap R and B 
        if (img.getPixelFormat() == HPF_A8R8G8B8){
            HarePixelType<HPF_A8R8G8B8>* tmpData = (HarePixelType<HPF_A8R8G8B8>*)pSrcBuf;
            for (uint32 count = 0; count < img.getWidth() * img.getHeight(); ++count){
                uint8 tmp;
                tmp        = tmpData->r;
                tmpData->r = tmpData->b;
                tmpData->b = tmp;
                tmpData++;
            }
        }

        texPixelFormat = HPF_A8B8G8R8;

        for (uint32 y = 0; y < srcHeight; ++y){
            memcpy(pDestBuf + y * PO2Width * PixelFB, pSrcBuf + y * srcWidth * PixelFB, srcWidth * PixelFB);
        }

        glGenTextures(1, &glTexture);

        glBindTexture(GL_TEXTURE_2D, glTexture);

        glTexImage2D(GL_TEXTURE_2D, 
            0,
            GLTypeConverter::getGLInternalFormat(texPixelFormat), 
            width, 
            height, 
            0, 
            GLTypeConverter::toGLFormat(texPixelFormat), 
            GL_UNSIGNED_BYTE, 
            (GLvoid*)pDestBuf);

		//default filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
        glBindTexture(GL_TEXTURE_2D, 0);
#ifdef _DEBUG
		GLenum ret = glGetError();
#endif
        delete [] pDestBuf;


    }

    return true;
}

void GLTexture::reCreate()
{
    assert(doCreate());
}

void GLTexture::release()
{
    if(glTexture != 0){
        glDeleteTextures(1, &glTexture);
        glTexture = 0;
    }
	if (bIsRenderable){
		if (depthbuffer){
			glDeleteRenderbuffersEXT(1, &depthbuffer);
			depthbuffer = 0;
		}
		if (fbo){
			glDeleteFramebuffersEXT(1, &fbo);
			fbo = 0;
		}
	}
}

GLuint GLTexture::getGLTexture()
{
    return glTexture;
}
