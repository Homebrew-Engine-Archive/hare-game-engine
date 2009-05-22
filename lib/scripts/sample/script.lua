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

function game_init()
    local hareApp = hare.getHareApp()
    local params = hare.WindowParams()
    params.width = 800
    params.height = 600
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

    guiSys = hare.GUISystem()
    local layout = hare.importObject('/sample/layouts/test.layout')
    local theme = hare.importObject('/sample/themes/vista_style.theme')
    guiSys:setRoot(layout)
    guiSys:setTheme(theme)

end

function game_quit()
end
