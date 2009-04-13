#include "PCH.h"
#include "GLTypeConverter.h"



GLenum GLTypeConverter::toGLPrimtiveType(RenderUnit::RenderOperationType primtype)
{
    GLenum returnType;
    switch (primtype)
    {
    case RenderUnit::ROT_LINE_LIST:      returnType = GL_LINES;break;
    case RenderUnit::ROT_TRIANGLE_LIST:  returnType = GL_TRIANGLES;break;
    case RenderUnit::ROT_QUAD_LIST:      returnType = GL_QUADS;break;
    default:assert(false);
    }
    return returnType;
}

uint32 GLTypeConverter::countByPrimtType(GLenum primtype)
{
    switch (primtype)
    {
    case GL_LINES:      return 2;
    case GL_TRIANGLES:  return 3;
    case GL_QUADS:      return 4;
    default:assert(false);
    }
    return 0;
}

GLenum GLTypeConverter::toGLFormat(HarePixelFormat hpf)
{
    GLenum glfmt;
    switch(hpf)
    {
    case HPF_BYTE_LA:
        glfmt = GL_LUMINANCE_ALPHA;//假设小地址结尾
        break;
    case HPF_A8B8G8R8://this format diffrent from d3d format
        glfmt = GL_RGBA;
        break;
    default:
        glfmt = GL_NONE;
        assert(false);
    }
    return glfmt;
}

GLenum GLTypeConverter::getGLInternalFormat(HarePixelFormat format)
{
    GLenum glfmt;
    switch(format) 
    {
    case HPF_BYTE_LA:
        glfmt = GL_LUMINANCE8_ALPHA8;
        break;
    case HPF_A8B8G8R8:
        glfmt = GL_RGBA8;
        break;
    default:
        glfmt = GL_NONE;
        assert(false);
    }
    return glfmt;
}

