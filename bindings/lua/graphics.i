%module hare

%include "lua_fnptr.i"

%{
#include "graphics/Graphics.h"
using namespace hare_core;
using namespace hare_graphics;
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
	
	void move(int x, int y);
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

	void setBeginSceneListenFun(SWIGLUA_REF fn);
	
	void setRenderSceneListenFun(SWIGLUA_REF fn);
	
	void setEndSceneListenFun(SWIGLUA_REF fn);

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
	u32    hwnd;
	bool   bFullScreen;
	u32    width;
	u32    height;
	bool   bZbuffer;
	String title;
};

class RenderWindow
{
public:
	void setSceneManager(SceneManager* scene);

	SceneManager* getSceneManager();
	
	bool getIsMainWnd();
	
	virtual void create(const WindowParams& params) = 0;

	virtual void resize(u32 w, u32 h) = 0;

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

class Material
{	
public:
	virtual void frameMove() = 0;
};

class TextureMtrl : public Material
{
public:
	virtual void frameMove();
	void setTexture(Texture* tex);
	
};

class Shader
{
public:
	virtual void setMaterial(Material* m) = 0;
	virtual Material* getMaterial() = 0;
	virtual TextureMtrl* getTextureMtrl() = 0;
};

class SimpleShader : public Shader
{
public:
	virtual void setMaterial(Material* m);
	virtual Material* getMaterial();
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

class Canvas
{
public:
	void drawLine(int x1, int y1, int x2, int y2, u32 color, f32 z = 0.f);
	void drawRect(int l, int t, int r, int b, u32 color, f32 z = 0.f);
	void drawText(int x, int y, const String& text);
	void drawImage(int x, int y, Shader* shader, f32 z = 0.f);
	
};

Canvas* getCanvas();
