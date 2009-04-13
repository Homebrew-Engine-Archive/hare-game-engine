#ifndef _GLTYPECONVERTER_H_
#define _GLTYPECONVERTER_H_

class GLTypeConverter
{
public:
    static GLenum toGLPrimtiveType(RenderUnit::RenderOperationType primtype);
    static uint32 countByPrimtType(GLenum primtype);
    static GLenum toGLFormat(HarePixelFormat hpf);
    static GLenum getGLInternalFormat(HarePixelFormat format);
};


#endif
