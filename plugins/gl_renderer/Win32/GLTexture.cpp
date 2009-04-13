#include "PCH.h"
#include "GLTexture.h"
#include "GLTypeConverter.h"


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

    glBindTexture(GL_TEXTURE_2D, glTexture);

    GLRenderSystem::getSingletonPtr()->clear(GLRenderSystem::getSingletonPtr()->getCurRenderWindow()->getWindowParams().bZbuffer);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 

    //NOTE!! glOrtho funcation last tow args is used as negative
    if (projectionWidth <= projectionHeight)                                                                    //near far
        glOrtho(-50.0, 50.0, -50.0*(GLfloat)projectionHeight/(GLfloat)projectionWidth, 50.0*(GLfloat)projectionHeight/(GLfloat)projectionWidth,-1.0,1.0); 
    else
        glOrtho(-50.0*(GLfloat)projectionWidth/(GLfloat)projectionHeight, 50.0*(GLfloat)projectionWidth/(GLfloat)projectionHeight, -50.0, 50.0,-1.0,1.0); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

    glViewport(0, 0, projectionWidth, projectionHeight);

}

void GLTexture::inactive()
{
    assert(bIsRenderable);

    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, 0, width, height, 0);
}

void GLTexture::upload(const Image& img, uint32 destX, uint32 destY)
{

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

        ZeroMemory(data, width * height * PixelFB);

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
            (GLvoid*)data);

        delete [] data;

		if (bIsRenderable){
			glGenFramebuffersEXT(1, &fbo);
    
			if (GLRenderSystem::getSingletonPtr()->getCurRenderWindow()->getWindowParams().bZbuffer){
				
			}
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
}

GLuint GLTexture::getGLTexture()
{
    return glTexture;
}
