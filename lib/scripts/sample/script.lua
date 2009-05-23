guiSys = nil
mouse = nil

function onBeginScene()
    mouse:capture()
    collectgarbage()
end

function onRenderScene()
    guiSys:render()
    hare.getCanvas():drawText(100, 100, tostring(hare.getTimer():getFPS()))
end

function onEndScene()
end

function onMousePressed(event, button)
    print("mouse pressed, button : ", button)
end


function onCreate(this)
    local hareApp = hare.getHareApp()
    local params = hare.WindowParams()
    params.width = 640
    params.height = 480
    params.bFullScreen = false
    params.bZbuffer = false
    local window = hareApp:createRenderWindow(params)
    local scene = hareApp:createSceneManager()
    window:setSceneManager(scene)

    local inputManager = hare.createLuaInputManager(window)
    print(swig_type(inputManager))
    mouse = inputManager:createMouse(true)
    local mouseListener = hare.LuaMouseListener()
    mouseListener:setMousePressedCallback(onMousePressed)
    mouse:setListener(mouseListener)

    local listener = hare.LuaSceneListener()
    listener:setBeginSceneCallback(onBeginScene)
    listener:setRenderSceneCallback(onRenderScene)
    listener:setEndSceneCallback(onEndScene)
    scene:setSceneListener(listener)

    local layout = hare.importObject('/sample/layouts/test.layout')
    print("layout : ", swig_type(layout))
    local theme = hare.importObject('/sample/themes/vista_style.theme')
    print("theme : ", swig_type(theme))
    guiSys = hare.GUISystem()
    guiSys:setRoot(layout)
    guiSys:setTheme(theme)

    local font = hare.importObject('/editor/default.font')
    hare.getCanvas():setFont(font)

end

function onDestroy(this)
    guiSys:setRoot(nil)
    guiSys:setTheme(nil)
    guiSys = nil
    mouse = nil
end
