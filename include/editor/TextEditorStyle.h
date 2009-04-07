//***************************************************************
//  File:    TextEditorStyle.h
//  Data:    01/03/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _TEXTEDITORSTYLE_H_
#define _TEXTEDITORSTYLE_H_

#include "EditorPrerequisites.h"
#include "core/Core.h"
#include <wx/string.h>
#include <wx/hashmap.h>

namespace hare_editor
{
    using namespace hare_core;

    class OptionColour : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(OptionColour)
    public:
        String name;
        s32 value;
        String fore;
        String back;
        u8 bold;
        u8 italics;
        u8 underlined;
    };

    class OptionSet : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(OptionSet)
    public:
        String lang;
        s32 lexer;
        OptionColour::Array colours;
        StringVector keywords;
        StringVector fileMasks;
        String sampleCode;
    };

    WX_DECLARE_STRING_HASH_MAP(OptionSet::Ptr, OptionSetsMap);

    typedef wxString HighlightLanguage;
    #define HL_AUTO wxT(" ")
    #define HL_NONE wxT("  ")

    class TextEditorStyle
    {
    public:
        TextEditorStyle();

        wxArrayString getAllLanguages();

        HighlightLanguage getLanguageFromFileName(const wxString& filename);

        HighlightLanguage apply(TextEditorPage* editor, HighlightLanguage lang = HL_AUTO);
        void apply(HighlightLanguage lang, wxScintilla* control);

        bool addOption(HighlightLanguage lang, OptionColour* option);
        OptionColour* getOptionByName(HighlightLanguage lang, const wxString& name);
        OptionColour* getOptionByValue(HighlightLanguage lang, int value);
        OptionColour* getOptionByIndex(HighlightLanguage lang, int index);
        int getOptionCount(HighlightLanguage lang);

        void save();
        bool reset(HighlightLanguage lang);

    private:
        void load();
        void doApplyStyle(wxScintilla* control, int value, OptionColour* option);

        OptionSetsMap setsMap;
    };
}

#endif
