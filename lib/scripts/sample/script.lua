sprite1 = nil
sprite2 = nil
sceneListen = nil
par = nil
fnt = nil

lastTime = 0

function beginScene()
    if hare.getTime() - lastTime > 1 then
        lastTime = hare.getTime()
        print("fps:", hare.getTimer():getFPS(), lastTime)
    end
end

function renderScene()
	canvas = hare.getCanvas()
	canvas:setZ(0)
	canvas:setColor(0xffff0000)
	canvas:setZ(-0.1)
	canvas:drawRect(50, 50, 100, 100)
	canvas:setZ(-0.9)
	canvas:setColor(0xff00ff00)
	canvas:drawText(80, 80,'Hello hare~!!')
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
    sprite2 = hare.SimpleSprite()
    sprite2:loadFromImage("/sample/images/sample.png")
    s:addSprite(sprite2)    
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
    --w = hareApp:createRenderWindow(p)
    --w:setSceneManager(s)
    
    
	tm = hare.getTextManager() 
	fnt = tm:createFont("comic.ttf", 50) 
	canvas = hare.getCanvas()
	canvas:setFont(fnt)

end

function quit()
    sprite1 = nil
    sprite2 = nil
    sceneListen = nil
    par = nil
    fnt = nil
end
