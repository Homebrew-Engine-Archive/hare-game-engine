%module hare

%{
#include "LuaSceneListener.h"
%}

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();

	virtual void beginScene() = 0;

	virtual void renderScene() = 0;

	virtual void endScene() = 0;
};

class SimpleSprite : public Sprite
{
public:
	SimpleSprite();
	virtual ~SimpleSprite();

	virtual void beginScene();

	virtual void renderScene();

	virtual void endScene();
	
	void loadFromImage(const String& filename);
	
	void move(float x, float y);
};

class SceneListenerBase
{
public:
	virtual void beginScene() = 0;
	virtual void renderScene() = 0;
	virtual void endScene() = 0;
};

class LuaSceneListener : public SceneListenerBase
{
public:
	virtual void beginScene();
	virtual void renderScene();
	virtual void endScene();

	void setBeginSceneListenFunc(SWIGLUA_REF fn);
	
	void setRenderSceneListenFunc(SWIGLUA_REF fn);
	
	void setEndSceneListenFunc(SWIGLUA_REF fn);

};

class SceneManager
{
public:
	void addSprite(Sprite* sprite);

	void removeSprite(Sprite* sprite);
	
	void setSceneListener(SceneListenerBase* listener);
};

struct WindowParams
{
	//WindowHandle hwnd;
	bool bFullScreen;
	uint32 width;
	uint32 height;
	bool bZbuffer;
	String title;
};

class RenderWindow
{
public:
	void setSceneManager(SceneManager* scene);

	SceneManager* getSceneManager();
	
	bool getIsMainWnd();
	
	virtual void create(const WindowParams& params) = 0;

	virtual void resize(uint32 w, uint32 h) = 0;

	virtual void swapBuffer() = 0;

	virtual void destoryWindow() = 0;
};	

class Texture
{
public:
	void createFromFile(const String& filename);
	virtual void reCreate() = 0;
	virtual void release() = 0;
};

class StandardMtrl
{	
public:
	virtual void frameMove() = 0;
};

class TextureMtrl : public StandardMtrl
{
public:
	virtual void frameMove();
	void setTexture(Texture* tex);
	
};

class Shader
{
public:
	virtual void setMaterial(StandardMtrl* m) = 0;
	virtual StandardMtrl* getMaterial() = 0;
	virtual TextureMtrl* getTextureMtrl() = 0;
};

class SimpleShader : public Shader
{
public:
	virtual void setMaterial(StandardMtrl* m);
	virtual StandardMtrl* getMaterial();
	virtual TextureMtrl* getTextureMtrl();
};

class HareApp
{
public:
	void loadPlugin(const String& moduleName);

	void freePlugin();

	RenderWindow* createRenderWindow(const WindowParams& params);

	SceneManager* createSceneManager();
	
	Texture* createTexture();

	void hareRun();
};

HareApp* getHareApp();

class Font : public Object
{
private:
	Font();
};

class Canvas
{
public:
	void drawLine(float x1, float y1, float x2, float y2);
	void drawRect(float l, float t, float r, float b);
	void drawText(int x, int y, const String& text);
	void drawImage(int x, int y, Material* mtrl);
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

class Particle : public Object
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

	virtual PointF getPosition();
	virtual void setPosition(float x, float y);
};

class AnimFrame : public Object
{
public:
    AnimFrame();
    virtual ~AnimFrame();
    
    void setMaterial(Material* m);
    Material* getMaterial();

public:
    float frameTime;
    RectF rectUV;
};

class Animation : public Object
{
public:
    Animation();
    virtual ~Animation();

    void addFrame(AnimFrame* frame);
    void render();
    void move(float dx, float dy);
    void moveTo(float x, float y);

};

