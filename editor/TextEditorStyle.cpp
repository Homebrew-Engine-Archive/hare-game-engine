//***************************************************************
//  File:    TextEditorStyle.cpp
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
#include "PCH.h"
#include "TextEditorStyle.h"
#include "TextEditorPage.h"
#include "EditorManager.h"
#include "ConfigManager.h"
#include <wx/dir.h>
#include <wx/wxscintilla.h>

namespace hare_editor
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(OptionColour, Object, 0)
    {
        HARE_META(name, String)
        HARE_META(value, s32)
        HARE_META(fore, String)
        HARE_META(back, String)
        HARE_ENUM(bold, u8, Bool)
        HARE_ENUM(italics, u8, Bool)
        HARE_ENUM(underlined, u8, Bool)
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(OptionSet, Object, 0)
    {
        HARE_META(lang, String)
        HARE_META(lexer, s32)
        HARE_OBJ_ARRAY(colours, OptionColour)
        HARE_META_ARRAY(keywords, String)
        HARE_META_ARRAY(fileMasks, String)
        HARE_META(sampleCode, String)
    }

    TextEditorStyle::TextEditorStyle()
    {
#if 1
        #define MAKE_OptionColour(NAME,VALUE,FORE,BACK,BOLD,ITALICS,UNDERLINED) \
        {   OptionColour::Ptr col = new OptionColour;\
            col->name = NAME;\
            col->value = VALUE;\
            col->fore = FORE;\
            col->back = BACK;\
            col->bold = BOLD;\
            col->italics = ITALICS;\
            col->underlined = UNDERLINED;\
            opt->colours.push_back(col);\
        }

        #define MAKE_RGB(r,g,b) StringUtil::format("RGB(%d,%d,%d)", r, g, b)

        // make lexer_lua.xml for lua
        {
            OptionSet::Ptr opt = new OptionSet;
            opt->lang = "lua";
            opt->fileMasks.push_back("*.lua");
            opt->lexer = wxSCI_LEX_LUA;
            opt->keywords.push_back("and break do else elseif end false for function global if "
                "in local nil not or repeat return then true until while");
            opt->keywords.push_back("_ALERT _ERRORMESSAGE _PROMPT _VERSION "
                "assert collectgarbage create error gcinfo globals loadfile loadstring metatable "
                "next nexti pcall print rawget rawset require tonumber tostring type unpack xpcall yield");
            opt->keywords.push_back("abs acos asin atan atan2 byte ceil char concat cos deg exp "
                "find floor foreach foreachi format frexp getn gfind gsub insert ldexp len log log10 lower "
                "max min mod pi rad random randomseed rep remove sin sort sqrt sub tan upper");
            opt->keywords.push_back("clock close date difftime execute exit flush getenv "
                "input open output popen read remove rename seek setlocale stdin stderr stdout "
                "time tmpfile tmpname write");

            MAKE_OptionColour("Default", 0,  MAKE_RGB(0,0,0),        MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Comment", 1,  MAKE_RGB(160,160,160),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Comment", 2,  MAKE_RGB(160,160,160),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Comment", 3,  MAKE_RGB(160,160,160),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Number",  4,  MAKE_RGB(240,0,240),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 5,  MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 13, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 14, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 15, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 16, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 17, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 18, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 19, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",  6,  MAKE_RGB(255,0,255),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",  7,  MAKE_RGB(255,0,255),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",  8,  MAKE_RGB(255,0,255),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",  12, MAKE_RGB(255,0,255),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Operator",10, MAKE_RGB(255,0,0),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Matching brace highlight",
                34, MAKE_RGB(0,0,0),        MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("No matching brace highlight",
                35, MAKE_RGB(255,0,0),      MAKE_RGB(255,0,0),     1, 0, 0);

            opt->saveToXml("lexers/lexer_lua.xml");
        }

        // make lexer_xml.xml for xml
        {
            OptionSet::Ptr opt = new OptionSet;
            opt->lang = "xml";
            opt->fileMasks.push_back("*.xml");
            opt->lexer = wxSCI_LEX_XML;

            MAKE_OptionColour("Default",         0,  MAKE_RGB(0,0,0),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Tag",             1,  MAKE_RGB(128,0,128),MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Tag",             2,  MAKE_RGB(128,0,128),MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Attribute name",  3,  MAKE_RGB(0,0,0),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Attribute name",  4,  MAKE_RGB(0,0,0),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Attribute value", 5,  MAKE_RGB(0,0,255),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Attribute value", 6,  MAKE_RGB(0,0,255),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Attribute value", 7,  MAKE_RGB(0,0,255),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Attribute value", 8,  MAKE_RGB(0,0,255),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Comment",         9,  MAKE_RGB(0,128,0),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Entity",          10, MAKE_RGB(255,69,0), MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("XML Start",       12, MAKE_RGB(128,0,128),MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("XML End",         13, MAKE_RGB(128,0,128),MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("CDATA Section",   17, MAKE_RGB(0,0,0),    MAKE_RGB(255,255,255), 0, 1, 0);

            opt->saveToXml("lexers/lexer_xml.xml");
        }
#endif
        load();
    }

    wxArrayString TextEditorStyle::getAllLanguages()
    {
        wxArrayString ret;
        for (OptionSetsMap::iterator it = setsMap.begin(); it != setsMap.end(); ++it)
        {
            wxString lang = wxString::FromUTF8(it->second->lang.c_str());
            if (!lang.IsEmpty())
                ret.Add(lang);
        }
        ret.Sort();
        return ret;
    }

    HighlightLanguage TextEditorStyle::getLanguageFromFileName(const wxString& filename)
    {
        wxString lfname = filename.Lower();

        for (OptionSetsMap::iterator it = setsMap.begin(); it != setsMap.end(); ++it)
        {
            for (u32 i = 0; i < it->second->fileMasks.size(); ++i)
            {
                wxString mask = wxString::FromUTF8(it->second->fileMasks[i].c_str());
                if (lfname.Matches(mask))
                    return it->first;
            }
        }
        return HL_NONE;
    }

    HighlightLanguage TextEditorStyle::apply(TextEditorPage* editor, HighlightLanguage lang)
    {
        if (!editor)
            return HL_NONE;

        if (lang == HL_AUTO)
            lang = getLanguageFromFileName(editor->getFileName());

        apply(lang, editor->getControl());

        return lang;
    }

    void TextEditorStyle::apply(HighlightLanguage lang, wxScintilla* control)
    {
        if (!control)
            return;

        control->StyleClearAll();

        if (lang == HL_NONE)
            return;

        // first load the default colours to all styles (ignoring some built-in styles)
        OptionColour* defaults = getOptionByName(lang, wxT("Default"));
        if (defaults)
        {
            for (int i = 0; i < wxSCI_STYLE_MAX; ++i)
            {
                if (i < 33 || i > 39)
                    doApplyStyle(control, i, defaults);
            }
        }

        control->StyleSetForeground(wxSCI_STYLE_LINENUMBER, wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

        OptionSet::Ptr curSet = setsMap[lang];

        for (u32 i = 0; i < curSet->colours.size(); ++i)
        {
            OptionColour::Ptr opt = curSet->colours[i];
            doApplyStyle(control, opt->value, opt);
        }

        control->SetLexer(curSet->lexer);
        control->SetStyleBits(control->GetStyleBitsNeeded());
        for (u32 i = 0; i <= wxSCI_KEYWORDSET_MAX && i < curSet->keywords.size(); ++i)
        {
            control->SetKeyWords(i, wxString::FromUTF8(curSet->keywords[i].c_str()));
        }
        control->Colourise(0, -1);
    }

    bool TextEditorStyle::addOption(HighlightLanguage lang, OptionColour* option)
    {
        if (lang == HL_NONE)
            return false;
        if (getOptionByValue(lang, option->value))
            return false;

        OptionSet::Ptr curSet = setsMap[lang];
        curSet->colours.push_back(new OptionColour(*option));
        return true;
    }

    OptionColour* TextEditorStyle::getOptionByName(HighlightLanguage lang, const wxString& name)
    {
        if (lang == HL_NONE)
            return 0;
        OptionSet::Ptr curSet = setsMap[lang];
        for (unsigned int i = 0; i < curSet->colours.size(); ++i)
        {
            OptionColour::Ptr opt = curSet->colours[i];
            if (wxString::FromUTF8(opt->name.c_str()) == name)
                return opt;
        }
        return 0;
    }

    OptionColour* TextEditorStyle::getOptionByValue(HighlightLanguage lang, int value)
    {
        if (lang == HL_NONE)
            return 0;
        OptionSet::Ptr curSet = setsMap[lang];
        for (u32 i = 0; i < curSet->colours.size(); ++i)
        {
            OptionColour::Ptr opt = curSet->colours[i];
            if (opt->value == value)
                return opt;
        }
        return 0;
    }

    OptionColour* TextEditorStyle::getOptionByIndex(HighlightLanguage lang, int index)
    {
        if (lang == HL_NONE)
            return 0;
        OptionSet::Ptr curSet = setsMap[lang];

        if (index > 0 && index < (int)curSet->colours.size())
            return curSet->colours[index];
        return 0;
    }

    int TextEditorStyle::getOptionCount(HighlightLanguage lang)
    {
        OptionSet::Ptr curSet = setsMap[lang];
        return (int)curSet->colours.size();
    }

    void TextEditorStyle::save()
    {
        for (OptionSetsMap::iterator it = setsMap.begin(); it != setsMap.end(); ++it)
        {
            it->second->saveToXml("lexer_" + it->second->lang + ".xml");
        }
    }

    bool TextEditorStyle::reset(HighlightLanguage lang)
    {
        String language = lang.ToUTF8().data();
        OptionSet::Ptr org = (OptionSet*)Object::loadFromXml("lexer_" + language + ".org");

        if (!org)
            return false;

        setsMap[lang] = org;
        return true;
    }

    void TextEditorStyle::load()
    {
        wxDir dir;
        wxString filename;
        static const String path = "lexers/";

        wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/lexers/");

        if (dir.Open(fullPath))
        {
            bool ok = dir.GetFirst(&filename, wxT("lexer_*.xml"), wxDIR_FILES);
            while (ok)
            {
                String name = path + filename.ToUTF8().data();
                OptionSet::Ptr opt = (OptionSet*)Object::importObject(name);
                if (opt)
                {
                    wxString name = wxString::FromUTF8(opt->lang.c_str());
                    setsMap[name] = opt;
                }
                ok = dir.GetNext(&filename);
            }
        }
    }

    void TextEditorStyle::doApplyStyle(wxScintilla* control, int value, OptionColour* option)
    {
        wxColour fore(wxString::FromUTF8(option->fore.c_str()));
        wxColour back(wxString::FromUTF8(option->back.c_str()));
        if (fore != wxNullColour)
            control->StyleSetForeground(value, fore);
        if (back != wxNullColour)
            control->StyleSetBackground(value, back);

        control->StyleSetBold(value, option->bold ? true : false);
        control->StyleSetItalic(value, option->italics ? true : false);
        control->StyleSetUnderline(value, option->underlined ? true : false);

    }

}
