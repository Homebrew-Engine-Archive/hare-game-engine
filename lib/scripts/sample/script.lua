scene = nil
window = nil
guiSys = nil

function beginScene()
end

function renderScene()
    guiSys:render()
end

function endScene()
end

function onCreate(this)
    local hareApp = hare.getHareApp()
    local params = hare.WindowParams()
    params.width = 640
    params.height = 480
    params.bFullScreen = false
    params.bZbuffer = false
    window = hareApp:createRenderWindow(params)
    scene = hareApp:createSceneManager()
    window:setSceneManager(scene)   

    local listener = hare.LuaSceneListener()
    listener:setBeginSceneCallback(beginScene)
    listener:setRenderSceneCallback(renderScene)
    listener:setEndSceneCallback(endScene)
    scene:setSceneListener(listener)

    local layout = hare.importObject('/sample/layouts/test.layout')
    print("layout : ", swig_type(layout))
    local theme = hare.importObject('/sample/themes/vista_style.theme')
    print("theme : ", swig_type(theme))
    guiSys = hare.GUISystem()
    guiSys:setRoot(layout)
    guiSys:setTheme(theme)
end

function onDestroy(this)
    guiSys:setRoot(nil)
    guiSys:setTheme(nil)
end
