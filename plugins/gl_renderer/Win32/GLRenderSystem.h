#ifndef _GLRENDERSYSTEM_H_
#define _GLRENDERSYSTEM_H_

class GLRenderSystem : public RenderSystem
{
public:
    GLRenderSystem();
    virtual ~GLRenderSystem();

    virtual void initalize();
    virtual void release();

    virtual void beginFrame();
    virtual void render();
    virtual void render(RenderUnit* operation);
    virtual void endFrame();
    virtual void clear(bool z);

    virtual void setShaderParams(const ShaderParams& shaderParams);
    virtual void setTextureStage(const TextureStage& textureStage);

    virtual Texture* createTexture();

protected:
    void writeBuffer(GLenum type, Vertex* buffer, uint32 count);
    void makeGLMatrix(GLfloat gl_matrix[16], const Matrix4& m);
};


#endif
