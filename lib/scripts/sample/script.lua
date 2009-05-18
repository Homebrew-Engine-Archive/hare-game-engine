sprite1 = nil
sprite2 = nil
sceneListen = nil
par = nil
fnt = nil
w   = nil
x = 0
y = 0
i = 1
lastTime = 0

function beginScene()
    --[[w:moveCameraTo(x, y)
    x = x + 50 * i * hare.getTimer():getDeltaTime()
    y = y + 50 * i * hare.getTimer():getDeltaTime()
    if x > 100 then
        i = -1
    end
    if x < -100 then
        i = 1
    end]]
end

function renderScene()
	canvas = hare.getCanvas()
	canvas:setZ(0)
	canvas:setColor(0xffff0000)
	canvas:setZ(-0.1)
	canvas:drawRect(0, 0, 100, 100)
	canvas:setZ(-0.9)
	canvas:setColor(0xff00ff00)
	canvas:drawText(80, 80, tostring(hare.getTimer():getFPS()))
	canvas:setColor(0xffffffff)
	canvas:setZ(0)
end

function endScene()
end

function init()
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
    listener:setBeginSceneListenFunc(beginScene)
    listener:setRenderSceneListenFunc(renderScene)
    listener:setEndSceneListenFunc(endScene)
    s:setSceneListener(listener)
    
    --p = hare.WindowParams()
    --p.width = 800
    --p.height = 600
    --p.bFullScreen = false
    --p.bZbuffer = true
    --w2 = hareApp:createRenderWindow(p)
    --w2:setSceneManager(s)

    tm = hare.getTextManager() 
    fnt = tm:createFont("/editor/comic.ttf", 30) 
    canvas = hare.getCanvas()
    canvas:setFont(fnt)

end

function quit()
    sprite1 = nil
    sprite2 = nil
    sceneListen = nil
    par = nil
    fnt = nil
    w = nil
end
