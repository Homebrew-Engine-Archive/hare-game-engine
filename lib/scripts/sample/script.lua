guiSys = nil
mouse = nil

--*******************************************
--  Scene listener callbacks
--*******************************************
function onBeginScene()
    mouse:capture()
end

function onRenderScene()
    guiSys:render()
    hare.getCanvas():drawText(100, 100, tostring(hare.getTimer():getFPS()))
end

function onEndScene()
end

--*******************************************
--  Mouse listener callbacks
--*******************************************
function onMousePressed(event, button)
    print("mouse pressed, button : ", button)
end

--*******************************************
--  Script entry callbacks
--*******************************************
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
    local theme = hare.importObject('/sample/themes/vista_style.theme')
    print("layout : ", swig_type(layout))
    print("theme : ", swig_type(theme))
    guiSys = hare.GUISystem()
    guiSys:setRoot(layout)
    guiSys:setTheme(theme)

    local font = hare.importObject('/editor/default.font')
    hare.getCanvas():setFont(font)

    -- Collect garbage right now, very useful for debugging
    collectgarbage()
end

function onDestroy(this)
    guiSys:setRoot(nil)
    guiSys:setTheme(nil)
end
