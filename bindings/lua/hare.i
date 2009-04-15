%include "std_string.i"
%include "typemaps.i"
%include "lua_fnptr.i"


%{

#include "core/Core.h"
#include "graphics/Graphics.h"
using namespace hare;

%}

%include core.i
%include graphics.i
