sprite = nil
sceneListen = nil
shader = nil

function beginScene()
end

function renderScene()
	canvas = hare.getCanvas()
	canvas:drawRect(50, 50, 100, 100, 0xffff0000)
end

function endScene()

end

function init()
    hareApp = hare.getHareApp()
    p = hare.WindowParams()
    p.hwnd = 0
    p.width = 800
    p.height = 600
    p.bFullScreen = false
    p.bZbuffer = true
    w = hareApp:createRenderWindow(p)
    s = hareApp:createSceneManager()
    w:setSceneManager(s)
    sprite = hare.SimpleSprite()
    sprite:loadFromImage("logo.png")
    s:addSprite(sprite)
    listener = hare.LuaSceneListener()
    listener:setBeginSceneListenFun(beginScene)
    listener:setRenderSceneListenFun(renderScene)
    listener:setEndSceneListenFun(endScene)
    s:setSceneListener(listener)
end

function quit()
    sprite = nil
end
