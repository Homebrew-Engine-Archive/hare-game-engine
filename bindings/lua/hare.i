%include "std_string.i"
%include "typemaps.i"
%include "lua_fnptr.i"


%{

#include "core/Core.h"
using namespace hare_core;

#include "graphics/Graphics.h"
using namespace hare_graphics;

%}

%include core.i
%include graphics.i