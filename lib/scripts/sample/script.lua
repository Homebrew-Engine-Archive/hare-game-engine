sprite1 = nil
sprite2 = nil
sceneListen = nil
--shader = nil




function beginScene()
    --sprite:move(1,1)
    t = hare.getTimer()
    print("fps"..tostring(t:getFPS()))
end

function renderScene()
	canvas = hare.getCanvas()
	--canvas:drawImage(500, 600, shader)
	canvas:drawRect(50, 50, 100, 100, 0xffff0000)
	canvas:drawLine(0,0, 800, 600, 0xffffffff, 1)
	canvas:drawRect(50, 50, 600, 500, 0xff00ff00, 0.5)
	canvas:drawText(0,0,'日')
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
    sprite1 = hare.SimpleSprite()
    sprite1:loadFromImage("xueren.png")
    s:addSprite(sprite1)
    sprite2 = hare.SimpleSprite()
    sprite2:loadFromImage("yu.png")
    s:addSprite(sprite2)    
    listener = hare.LuaSceneListener()
    listener:setBeginSceneListenFun(beginScene)
    listener:setRenderSceneListenFun(renderScene)
    listener:setEndSceneListenFun(endScene)
    s:setSceneListener(listener)
    
    p = hare.WindowParams()
    p.hwnd = 0
    p.width = 200
    p.height = 200
    p.bFullScreen = false
    p.bZbuffer = true
    w = hareApp:createRenderWindow(p)
    s = hareApp:createSceneManager()
    w:setSceneManager(s)
		s:addSprite(sprite2)
    
    --tex = hareApp:createTexture()
    --tex:createFromFile("logo.png")
    --texMtrl = hare.TextureMtrl()
    --texMtrl:setTexture(tex)
    --shader = hare.SimpleShader()
    --shader:setMaterial(texMtrl)
end

function quit()
    sprite1 = nil
    sprite2 = nil
end
