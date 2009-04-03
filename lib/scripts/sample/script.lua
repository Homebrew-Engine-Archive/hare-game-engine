sprite1 = nil
sprite2 = nil
sceneListen = nil
pp = nil

lastTime = 0

a = {[{100, 200}] = { 300, 400}, 200, { 300, 500}, abc = "abc"}

function beginScene()
    if hare.getTime() - lastTime > 1 then
        lastTime = hare.getTime()
        print("fps:", hare.getTimer():getFPS())
    end
    pp:frameMove()
end

function renderScene()
	canvas = hare.getCanvas()
	--canvas:drawImage(500, 600, shader)
	canvas:drawRect(50, 50, 100, 100)
	canvas:drawLine(0,0, 800, 600)
	canvas:drawRect(50, 50, 600, 500)
 
	canvas:drawText(50,50,'abcsfsdfhh')
	pp:render()
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
    --s:addSprite(sprite1)
    sprite2 = hare.SimpleSprite()
    sprite2:loadFromImage("yu.png")
    s:addSprite(sprite2)    
    listener = hare.LuaSceneListener()
    listener:setBeginSceneListenFunc(beginScene)
    listener:setRenderSceneListenFunc(renderScene)
    listener:setEndSceneListenFunc(endScene)
    s:setSceneListener(listener)

    --p = hare.WindowParams()
    --p.hwnd = 0
    --p.width = 200
    --p.height = 200
    --p.bFullScreen = false
    --p.bZbuffer = true
    --w = hareApp:createRenderWindow(p)
    --s = hareApp:createSceneManager()
    --w:setSceneManager(s)
	  --s:addSprite(sprite2)
	  --s:setSceneListener(listener)
	  
	  --p = hare.WindowParams()
    --p.hwnd = 0
    --p.width = 200
    --p.height = 200
    --p.bFullScreen = false
    --p.bZbuffer = false
    --w = hareApp:createRenderWindow(p)
    --s = hareApp:createSceneManager()
    --w:setSceneManager(s)
    --s:addSprite(sprite2)
	  --s:setSceneListener(listener)  
		
	tm = hare.getTextManager() 
	fnt = tm:createFont("comic.ttf", 20) 
	canvas = hare.getCanvas()
	canvas:setFont(fnt)
	
	pp = hare.Object_importObject('default.particle')
	pp:fireAt(300,300)
end

function quit()
    sprite1 = nil
    sprite2 = nil
    pp = nil
end
