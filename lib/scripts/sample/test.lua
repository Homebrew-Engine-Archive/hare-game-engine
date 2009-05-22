print("test.lua loaded!!!")

function onCreate(this)
    print("test.lua onCreate(this) called ", swig_type(this))
end

function onDestroy(this)
    print("test.lua onDestroy(this) called ", swig_type(this))
end
