%module hare

%{
#include "LuaSceneListener.h"
%}

%typemap(out) Sprite* { if ($1 && $owner) ($1)->addRef(); dynamicCastObject(L, $1, $owner); SWIG_arg++; }


class Sprite : public Object
{
public:
	Sprite();
	virtual ~Sprite();

	virtual void move(float dx, float dy);
	virtual void moveTo(float x, float y);
	virtual void render() = 0;
	virtual const PointF& getPosition();
	void setOrigoPos(float x, float y);
	void moveOrigoPos(float dx, float dy);
	const PointF& getOrigoPos();
	void setFaceX(bool b);
	void setFaceY(bool b);
	void setSpriteName(const String& name);
	const String& getSpriteName();
	void setColor(uint32 c);
	uint32 getColor();
	void setZ(float z);
	float getZ();
};

class SceneListener : public ReferenceCounted
{
public:
	virtual void beginScene() = 0;
	virtual void renderScene() = 0;
	virtual void endScene() = 0;

private:
    virtual void _doRelease();

};

class LuaSceneListener : public SceneListener
{
public:
	virtual void beginScene();
	virtual void renderScene();
	virtual void endScene();

	void setBeginSceneCallback(SWIGLUA_REF func);
	void setRenderSceneCallback(SWIGLUA_REF func);
	void setEndSceneCallback(SWIGLUA_REF func);
};

class SceneManager : public Object
{
public:
	SceneManager();
	virtual ~SceneManager();

	int addSprite(Sprite* sprite);
	void removeSprite(Sprite* sprite);
	void removeAllSprite();
	void setSceneListener(SceneListener* listener);
	Sprite* getSpriteByName(const String& name);
	Sprite* getSpriteByID(int id);
	int getSpriteCount();

};

class WindowParams
{
public:
    uint32 width;
    uint32 height;
    String title;
    bool fullScreen;
    bool hasZbuffer;
};

class RenderWindow
{
public:
    uint32 getWidth();
    uint32 getHeight();

	void setSceneManager(SceneManager* scene);

	SceneManager* getSceneManager();

	bool getIsMainWnd();

	virtual void create(const WindowParams& params) = 0;

	virtual void resize(uint32 w, uint32 h) = 0;

	virtual void swapBuffer() = 0;

	virtual void destoryWindow() = 0;

	void moveCamera(float dx, float dy);

    void moveCameraTo(float x, float y);
};

class Texture
{
public:
	void createFromFile(const String& filename);
	virtual void recreate() = 0;
	virtual void release() = 0;
};

class Material : public Object
{
public:
	virtual void frameMove() = 0;
	virtual TextureMtrl* getTextureMtrl() = 0;
	virtual Shader* getShader() = 0;
};

class StandardMtrl : public Material
{
};

class TextureMtrl : public StandardMtrl
{
public:
	virtual void frameMove();
	virtual TextureMtrl* getTextureMtrl();
	virtual Shader* getShader();
	void setTexture(Texture* tex);
};

class Shader : public Material
{
public:
	virtual void setMaterial(StandardMtrl* m) = 0;
	virtual StandardMtrl* getMaterial() = 0;
	virtual Shader* getShader();
};

class SimpleShader : public Shader
{
public:
    virtual void frameMove();
	virtual void setMaterial(StandardMtrl* m);
	virtual StandardMtrl* getMaterial();
	virtual TextureMtrl* getTextureMtrl();
};

class ParticleShader : public Shader
{
public:
    virtual void frameMove();
	virtual void setMaterial(StandardMtrl* m);
	virtual StandardMtrl* getMaterial();
	virtual TextureMtrl* getTextureMtrl();
};

class HareApp
{
public:
	RenderWindow* createRenderWindow(const WindowParams& params);

	SceneManager* createSceneManager();

	Texture* createTexture();

	void hareRun();
};

HareApp* getHareApp();

class Font : public Object
{
};

class Canvas
{
public:
	void drawLine(float x1, float y1, float x2, float y2);
	void drawRect(float l, float t, float r, float b);
	void drawRectFill(float l, float t, float r, float b);
	void drawText(float x, float y, const String& text);
	void drawImage(float x, float y, Material* mtrl);
	void setFont(Font* f);
	Font* getFont();
	void setColor(uint32 c);
	uint32 getColor();
	void setZ(float z);
	float getZ();
};

Canvas* getCanvas();

class TextManager
{
public:
	Font* createFont(const String& name, float size);
};

TextManager* getTextManager();

class Particle : public Sprite
{
public:
	Particle();
	virtual ~Particle();

	virtual void render() = 0;
	virtual void fire() = 0;
	virtual void fireAt(float x, float y) = 0;
	virtual void move(float offsetX, float offsetY) = 0;
	virtual void moveTo(float x, float y) = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;

	virtual const PointF& getPosition();
	virtual void setPosition(float x, float y);
	void   setMaterial(Material* mtrl);
};

class ImageSprite : public Sprite
{
public:
	ImageSprite();
	virtual ~ImageSprite();

	virtual void render();

	void loadFromImage(const String& filename);

	void loadFromMaterial(Material* m);

	void setUV(float l, float r, float t, float b);

	void setUV(const RectF& rect);

	Material* getMaterial();

};

class AnimFrame : public Object
{
public:
    AnimFrame();
    virtual ~AnimFrame();
	void setSprite(Sprite* s);
	Sprite* getSprite();

public:
	float frameTime;

};

class AnimationSprite : public Sprite
{
public:
    AnimationSprite();
    virtual ~AnimationSprite();

    int addFrame(AnimFrame* frame);

    AnimFrame* getFrame(int frameID);

    bool insertFrame(int frameID, AnimFrame* frame);

    bool removeFrame(int frameID);

    bool removeFrame(AnimFrame* frame);

    bool swapFrame(int frameID_1, int frameID_2);

    virtual void move(float dx, float dy);

    virtual void moveTo(float x, float y);

};

class ComponentSprite : public Sprite
{
public:
	ComponentSprite();
	virtual ~ComponentSprite();

	virtual void move(float dx, float dy);

	virtual void moveTo(float x, float y);

	virtual void render();

	int addPart(Sprite* s);

	int getPartID(Sprite* s);

	bool removePart(int partID);

	bool removePart(Sprite* s);

	//NB: swap render sequence
	bool swapPart(int partID_1, int partID_2);

	Sprite* getPart(int partID);

	Sprite* getPart(const String& name);

	uint32 getPartCount();

};
