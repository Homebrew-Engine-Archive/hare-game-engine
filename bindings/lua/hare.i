%include "std_string.i"
%include "typemaps.i"
%include "lua_fnptr.i"


%{

#include "core/Core.h"
#include "graphics/Graphics.h"
#include "gui/GUI.h"
using namespace hare;

%}

%feature("ref")     Object  "$this->addRef();"
%feature("unref")   Object  "$this->decRef();"

%include core.i
%include graphics.i
%include gui.i
