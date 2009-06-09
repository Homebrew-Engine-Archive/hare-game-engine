from hare import *

guiSys = None

def onBeginScene():
    pass

def onRenderScene():
    global guiSys
    guiSys.render()
    pass

def onEndScene():
    pass

def onCreate(self):
    hareApp = getHareApp()
    params = WindowParams()
    params.width = 800
    params.height = 600
    params.bFullScreen = False
    params.bZbuffer = True
    window = hareApp.createRenderWindow(params)

    scene = hareApp.createSceneManager()
    window.setSceneManager(scene)

    listener = PySceneListener()
    listener.setBeginSceneCallback(onBeginScene)
    listener.setRenderSceneCallback(onRenderScene)
    listener.setEndSceneCallback(onEndScene)
    scene.setSceneListener(listener)

    layout = importObject('/sample/layouts/test.layout')
    theme = importObject('/sample/themes/test.theme')
    global guiSys
    guiSys = GUISystem()
    guiSys.setRoot(layout)
    guiSys.setTheme(theme)
    pass
    
def onDestroy(self):
    pass
