#ifndef _GLTEXTURE_H_
#define _GLTEXTURE_H_


class GLTexture : public Texture
{
public:
    GLTexture();

    virtual ~GLTexture();

    virtual void active();

    virtual void inactive();

    virtual void upload(const Image& img, uint32 destX = 0, uint32 destY = 0);

    virtual void download(Image& img, const RectN& rc = RectN(0,0,0,0));

    virtual void reCreate();

    virtual void release();

    GLuint  getGLTexture();

protected:
    bool doCreate();

private:
    GLuint glTexture;
	GLuint fbo;
	GLuint depthbuffer;

};


#endif
