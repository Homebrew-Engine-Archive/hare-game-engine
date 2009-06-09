print("test.lua loaded!!!")

button = nil

function onCreate(this)
    button = this
    hare.connectSelf(this, hare.uiEVT_LEFT_CLICK, onLeftButtonClick)
    print("test.lua onCreate(this) called ", swig_type(this))
end

function onDestroy(this)
    button = nil
    hare.disconnectSelf(this, hare.uiEVT_LEFT_CLICK)
    print("test.lua onDestroy(this) called ", swig_type(this))
end

function onLeftButtonClick(sender, event)
    collectgarbage()
    print(swig_type(sender), swig_type(event))
end
