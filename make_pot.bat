@echo off

@i18n\tool\wxrc.exe lib\editor_data\resources\*.xrc -g -o editor\resource_xrc.cpp

@i18n\tool\bin\xgettext.exe -C -n -k_ -o    i18n\editor.pot editor\*.cpp
@i18n\tool\bin\xgettext.exe -C -n -k_ -j -o i18n\editor.pot include\editor\*.h
@i18n\tool\bin\xgettext.exe -C -n -k_ -j -o i18n\editor.pot editor_app\*.cpp
@i18n\tool\bin\xgettext.exe -C -n -k_ -j -o i18n\editor.pot editor_app\*.h

@i18n\tool\bin\xgettext.exe -C -n -k_ -o    i18n\lua_debugger.pot plugins\lua_debugger\*.cpp
@i18n\tool\bin\xgettext.exe -C -n -k_ -j -o i18n\lua_debugger.pot plugins\lua_debugger\*.h

@pause
