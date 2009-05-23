%include "std_string.i"
%include "typemaps.i"
%include "lua_fnptr.i"


%{

#include "core/Core.h"
#include "graphics/Graphics.h"
#include "gui/GUI.h"
using namespace hare;

%}

%feature("ref")     ReferenceCounted  "$this->addRef();"
%feature("unref")   ReferenceCounted  "$this->decRef();"

%include core.i
%include graphics.i
%include gui.i
%include input.i
