#ifndef EDITOR_PREREQUISITES_H
#define EDITOR_PREREQUISITES_H

#include "core/Core.h"
#include "graphics/Graphics.h"
using namespace hare;

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   if defined(HARE_STATIC_LIB)
#   	define EDITOR_API
#   else
#       if defined(EDITOR_EXPORTS)
#           define EDITOR_API __declspec(dllexport)
#       else
#           define EDITOR_API __declspec(dllimport)
#       endif
#   endif
#elif HARE_PLATFORM == HARE_PLATFORM_LINUX || HARE_PLATFORM == HARE_PLATFORM_APPLE
#   if defined(HARE_GCC_VISIBILITY)
#       define EDITOR_API  __attribute__ ((visibility("default")))
#   else
#       define EDITOR_API
#   endif
#else
#   error "only support win32 & linux & apple"
#endif

namespace hare
{
    template <typename T> class TManager;
    class Manager;
    class ConfigManager;
    class ExplorerManager;
    class EditorPageManager;
    class PluginManager;
    class EditorPlugin;
    class EditorPage;
    class TextEditorPage;
    class EncodingDetector;
    class EditorEvent;
    class WizardResult;
    class FileSystemExplorer;
    class ProjectExplorer;
    class Project;
    class ProjectFile;
}

class wxTreeCtrl;
class wxListCtrl;
class wxMenuBar;
class wxAuiToolBar;
class wxFrame;
class wxFlatNotebook;
class wxFlatNotebookEvent;
class wxScintilla;
class wxPropertyGridManager;
class wxPropertyGridPage;
class wxScintillaEvent;
class wxListEvent;

#endif // EDITOR_PREREQUISITES_H
