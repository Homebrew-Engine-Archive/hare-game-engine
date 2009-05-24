window = nil
guiSys = nil
mouse = nil
keyboard = nil
mouseX = 0
mouseY = 0
--*******************************************
--  Scene listener callbacks
--*******************************************
function onBeginScene()
    mouse:getMouseState().width = window:getWidth()
    mouse:getMouseState().height = window:getHeight()
    mouse:capture()
    keyboard:capture()
end

function onRenderScene()
    guiSys:render()
    hare.getCanvas():drawText(10, 20, "FPS : "..hare.getTimer():getFPS())
    hare.getCanvas():drawText(10, 35, "X : "..mouseX.." Y : "..mouseY)
end

function onEndScene()
end

--*******************************************
--  Mouse listener callbacks
--*******************************************
function onMouseMoved(event)
    --print("mouse moved", event.state.buttons)
    --print(event.state.X.abs, event.state.X.rel)
    --print(event.state.Y.abs, event.state.Y.rel)
    mouseX = event.state.X.abs
    mouseY = event.state.Y.abs
end

function onMousePressed(event, button)
    print("mouse pressed, button : ", button)
end

function onMouseReleased(event, button)
    print("mouse released, button : ", button)
end

--*******************************************
--  Key listener callbacks
--*******************************************
function onKeyPressed(event)
    print("key pressed, keycode : ", event.key, event.text)
end

function onKeyReleased(event)
    print("key pressed, keycode : ", event.key, event.text)
end

--*******************************************
--  Script entry callbacks
--*******************************************
function onCreate(this)
   -- Create Window
    local hareApp = hare.getHareApp()
    local params = hare.WindowParams()
    params.width = 640
    params.height = 480
    params.bFullScreen = false
    params.bZbuffer = false
    window = hareApp:createRenderWindow(params)
    
    -- Create Scene
    local scene = hareApp:createSceneManager()
    window:setSceneManager(scene)

    -- Create SceneListener
    local listener = hare.LuaSceneListener()
    listener:setBeginSceneCallback(onBeginScene)
    listener:setRenderSceneCallback(onRenderScene)
    listener:setEndSceneCallback(onEndScene)
    scene:setSceneListener(listener)

    -- Create InputManager
    local inputManager = hare.createLuaInputManager(window)
    print(swig_type(inputManager))

    -- Mouse
    mouse = inputManager:createMouse(true)
    local mouseListener = hare.LuaMouseListener()
    mouseListener:setMouseMovedCallback(onMouseMoved)
    mouseListener:setMousePressedCallback(onMousePressed)
    mouseListener:setMouseReleasedCallback(onMouseReleased)
    mouse:setListener(mouseListener)

    mouse:getMouseState().width = window:getWidth()
    mouse:getMouseState().height = window:getHeight()

    -- Keyboard
    keyboard = inputManager:createKeyboard(true)
    local keyListener = hare.LuaKeyListener()
    keyListener:setKeyPressedCallback(onKeyPressed)
    keyListener:setKeyReleasedCallback(onKeyReleased)
    keyboard:setListener(keyListener)

    local layout = hare.importObject('/sample/layouts/test2.layout')
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
