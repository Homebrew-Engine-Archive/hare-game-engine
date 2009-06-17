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

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(OptionColour, Object, 0)
    {
        HARE_META(name, String)
        HARE_META(value, int32)
        HARE_META(fore, String)
        HARE_META(back, String)
        HARE_ENUM(bold, uint8, Bool)
        HARE_ENUM(italics, uint8, Bool)
        HARE_ENUM(underlined, uint8, Bool)
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(OptionSet, Object, 0)
    {
        HARE_META(lang, String)
        HARE_META(lexer, int32)
        HARE_OBJ_ARRAY(colours, OptionColour)
        HARE_META_ARRAY(keywords, String)
        HARE_META_ARRAY(fileMasks, String)
        HARE_META(sampleCode, String)
    }

    TextEditorStyle::TextEditorStyle()
    {
#if 0
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
            MAKE_OptionColour("Comment", 1,  MAKE_RGB(0,128,0),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Comment", 2,  MAKE_RGB(0,128,0),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Comment", 3,  MAKE_RGB(0,128,128),  MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Number",  4,  MAKE_RGB(255,128,0),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 5,  MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("Keyword", 13, MAKE_RGB(0,128,192),      MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("Keyword", 14, MAKE_RGB(128,0,255),      MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("Keyword", 15, MAKE_RGB(0,0,160),      MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("Keyword", 16, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 17, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 18, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keyword", 19, MAKE_RGB(0,0,255),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",  6,  MAKE_RGB(128,128,128),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",  7,  MAKE_RGB(128,128,128),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",  8,  MAKE_RGB(149,0,74),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",  12, MAKE_RGB(255,0,255),    MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Operator",10, MAKE_RGB(0,0,128),      MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Matching brace highlight",
                34, MAKE_RGB(0,0,0),        MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("No matching brace highlight",
                35, MAKE_RGB(255,0,0),      MAKE_RGB(255,0,0),     1, 0, 0);

            opt->saveToXml("/editor/lexers/lexer_lua.xml");
        }

        // make lexer_xml.xml for xml
        {
            OptionSet::Ptr opt = new OptionSet;
            opt->lang = "xml";
            opt->fileMasks.push_back("*.xml");
            opt->fileMasks.push_back("*.sprite");
            opt->fileMasks.push_back("*.layout");
            opt->fileMasks.push_back("*.font");
            opt->fileMasks.push_back("*.theme");
            opt->fileMasks.push_back("*.scene");
            opt->fileMasks.push_back("*.material");
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

            opt->saveToXml("/editor/lexers/lexer_xml.xml");
        }

        // make lexer_py.xml for py
        {
            OptionSet::Ptr opt = new OptionSet;
            opt->lang = "python";
            opt->fileMasks.push_back("*.py");
            opt->lexer = wxSCI_LEX_PYTHON;

            opt->keywords.push_back(
                "and assert break class continue def del elif else except "
                "exec finally for from global if import in is lambda None "
                "not or pass print raise return try while yield "
                "__import__ abs basestring bool callable chr classmethod "
                "cmp compile complex delattr dict dir divmod enumerate "
                "eval execfile file filter float frozenset getattr globals "
                "hasattr hash help hex id input int isinstance issubclass "
                "iter len list locals long map max min object oct open "
                "ord pow property range raw_input reduce reload repr "
                "reversed round set setattr slice sorted staticmethod "
                "str sum super tuple type type unichr unicode vars xrange "
                "zip "
                "apply buffer coerce intern "
                "__dict__ Ellipsis False True NotImplemented "
                "__class__ __bases__ __name__ "
                "exception Exception StandardError ArithmeticError "
                "LookupError EnvironmentError AssertionError "
                "AttributeError EOFError FloatingPointError IOError "
                "ImportError IndexError KeyError KeyboardInterrupt "
                "MemoryError NameError NotImplementedError OSError "
                "OverflowError ReferenceError RuntimeError "
                "StopIteration SyntaxError SystemError SystemExit "
                "TypeError UnboundLocalError UnicodeError "
                "UnicodeEncodeError UnicodeDecodeError "
                "UnicodeTranslateError ValueError WindowsError "
                "ZeroDivisionError Warning UserWarning "
                "DeprecationWarning PendingDeprecationWarning "
                "SyntaxWarning RuntimeWarning FutureWarning");

            opt->keywords.push_back(
                "sys gc weakref fpectl atexit types UserDict UserList UserString "
                "operator inspect traceback linecache pickle cPickle copy_reg "
                "shelve copy marshal warnings imp zipimport pkgutil modulefinder "
                "code codeop pprint repr new site user __builtin__ __main__ "
                "__future__ "
                "string re struct difflib fpformat StringIO cStringIO textwrap "
                "codecs encodings.idna unicodedata stringprep "
                "pydoc doctest unittest test test.test_support decimal math "
                "cmath random whrandom bisect collections heapq array sets "
                "itertools ConfigParser fileinput calendar cmd shlex "
                "os os.path dircache stat statcache statvfs filecmp subprocess "
                "popen2 datetime time sched mutex getpass curses curses.textpad "
                "curses.wrapper curses.ascii curses.panel getopt optparse tempfile "
                "errno glob fnmatch shutil locale gettext logging platform "
                "signal socket select thread threading dummy_thread dummy_threading "
                "Queue mmap anydbm dbhash whichdb bsddb dumbdbm zlib gzip bz2 "
                "zipfile tarfile readline rlcompleter "
                "posix pwd grp crypt dl dbm gdbm termios tty pty fcntl pipes "
                "posixfile resource nis syslog commands "
                "hotshot timeit "
                "webbrowser cgi cgitb urllib urllib2 httplib ftplib gopherlib "
                "poplib imaplib nntplib smtplib smtpd telnetlib urlparse "
                "SocketServer BaseHTTPServer SimpleHTTPServer CGIHTTPServer "
                "cookielib Cookie xmlrpclib SimpleXMLRPCServer DocXMLRPCServer "
                "asyncore asynchat "
                "formatter email email.Message email.Parser email.Generator "
                "email.Header email.Charset email.Encoders email.Errors "
                "email.Utils email.Iterators mailcap mailbox mhlib mimetools "
                "mimetypes MimeWriter mimify multifile rfc822 base64 binascii "
                "binhex quopri uu xdrlib netrc robotparser csv "
                "HTMLParser sgmllib htmllib htmlentitydefs xml.parsers.expat "
                "xml.dom xml.dom.minidom xml.dom.pulldom xml.sax "
                "xml.sax.handler xml.sax.saxutils xml.sax.xmlreader xmllib "
                "audioop imageop aifc sunau wave chunk colorsys rgbimg imghdr "
                "sndhdr ossaudiodev "
                "hmac md5 sha "
                "Tkinter Tix ScrolledText turtle "
                "parser symbol token keyword tokenize tabnanny pyclbr "
                "py_compile compileall dis pickletools distutils");

            MAKE_OptionColour("Default",         0,  MAKE_RGB(0,0,0),       MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Comment line",    1,  MAKE_RGB(0,128,0),     MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Number",          2,  MAKE_RGB(255,0,0),     MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("String",          3,  MAKE_RGB(128,128,128), MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Character",       4,  MAKE_RGB(128,128,128), MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Keywords",        5,  MAKE_RGB(0,0,255),     MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("Triple",          6,  MAKE_RGB(255,128,0),   MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Triple double",   7,  MAKE_RGB(0,0,255),     MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Class name",      8,  MAKE_RGB(0,0,0),       MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("Def name",        9,  MAKE_RGB(255,0,255),   MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Operator",        10, MAKE_RGB(0,0,128),     MAKE_RGB(255,255,255), 1, 0, 0);
            MAKE_OptionColour("Identifier",      11, MAKE_RGB(0,0,0),       MAKE_RGB(255,255,255), 0, 0, 0);
            MAKE_OptionColour("Comment block",   12, MAKE_RGB(128,0,128),MAKE_RGB(255,255,255), 0, 0, 0);

            opt->saveToXml("/editor/lexers/lexer_py.xml");
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
            for (uint32 i = 0; i < it->second->fileMasks.size(); ++i)
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

        for (uint32 i = 0; i < curSet->colours.size(); ++i)
        {
            OptionColour::Ptr opt = curSet->colours[i];
            doApplyStyle(control, opt->value, opt);
        }

        control->SetLexer(curSet->lexer);
        control->SetStyleBits(control->GetStyleBitsNeeded());
        for (uint32 i = 0; i <= wxSCI_KEYWORDSET_MAX && i < curSet->keywords.size(); ++i)
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
        for (uint32 i = 0; i < curSet->colours.size(); ++i)
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
            it->second->saveToXml("/editor/lexers/lexer_" + it->second->lang + ".xml");
        }
    }

    bool TextEditorStyle::reset(HighlightLanguage lang)
    {
        assert(false);
        return true;
    }

    void TextEditorStyle::load()
    {
        wxDir dir;
        wxString filename;
        static const String path = "/editor/lexers/";

        wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/editor/lexers/");

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
