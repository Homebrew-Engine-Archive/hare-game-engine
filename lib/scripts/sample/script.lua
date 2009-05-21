sprite1 = nil
sprite2 = nil
sceneListen = nil
par = nil
fnt = nil
w   = nil
mtrl = nil


x = 0
y = 0
i = 1
lastTime = 0

function beginScene()
    --w:moveCameraTo(x, y)
    x = x + 50 * i * hare.getTimer():getDeltaTime()
    y = y + 50 * i * hare.getTimer():getDeltaTime()
    if x > 100 then
        i = -1
    end
    if x < -100 then
        i = 1
    end
end

function renderScene()
	canvas = hare.getCanvas()
	canvas:setZ(0)
	canvas:setColor(0xffff0000)
	canvas:setZ(-0.1)
	canvas:drawImage(100, 100, mtrl)
	canvas:drawText(80, 80, tostring(hare.getTimer():getFPS()))
	canvas:setZ(-0.9)
	canvas:setColor(0xff00ff00)
	canvas:drawRect(50, 50, 100, 100)
	canvas:setColor(0xffffffff)
	canvas:setZ(0)
end

function endScene()
end

function game_init()
    hareApp = hare.getHareApp()
    p = hare.WindowParams()
    p.width = 800
    p.height = 600
    p.bFullScreen = false
    p.bZbuffer = false
    w = hareApp:createRenderWindow(p)
    s = hareApp:createSceneManager()
    w:setSceneManager(s)   

    listener = hare.LuaSceneListener()
    listener:setBeginSceneCallback(beginScene)
    listener:setRenderSceneCallback(renderScene)
    listener:setEndSceneCallback(endScene)
    s:setSceneListener(listener)
    
    p = hare.WindowParams()
    p.width = 600
    p.height = 400
    p.bFullScreen = false
    p.bZbuffer = true
    w2 = hareApp:createRenderWindow(p)
    w2:setSceneManager(s)
    
		p = hare.WindowParams()
    p.width = 400
    p.height = 300
    p.bFullScreen = false
    p.bZbuffer = true
    w2 = hareApp:createRenderWindow(p)
    w2:setSceneManager(s)

    tm = hare.getTextManager() 
    fnt = tm:createFont("/editor/comic.ttf", 10) 
    canvas = hare.getCanvas()
    canvas:setFont(fnt)

		mtrl = hare.importObject('/pig/pig0.material')
end

function game_quit()
    sprite1 = nil
    sprite2 = nil
    sceneListen = nil
    par = nil
    fnt = nil
    w = nil
    mtrl = nil
end
