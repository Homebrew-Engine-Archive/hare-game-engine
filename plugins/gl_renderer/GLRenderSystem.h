#ifndef _GLRENDERSYSTEM_H_
#define _GLRENDERSYSTEM_H_

class GLVertexBufferManager;

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

	void initalizeParam();
protected:
    void makeGLMatrix(GLfloat gl_matrix[16], const Matrix4& mat);
    void writeBuffer(GLenum type, Vertex* buffer, uint32 count);

protected:
	ShaderParams curShaderParams;

	TextureStage curTextureStage;

	GLuint  curRenderTexture;

	GLenum  PrimType;

	Matrix4 texMat;

	GLfloat gl_matrix[16];
};


#endif
