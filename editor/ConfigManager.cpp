//***************************************************************
//  File:    ConfigManager.cpp
//  Date:    10/26/2008
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
#include "EditorManager.h"
#include "ConfigManager.h"
#include <wx/intl.h>
#include <wx/font.h>
#include <wx/file.h>

namespace hare
{
    Object* Editor_importObject(const wxString& fileName)
    {
        wxFile file;
        if (!file.Open(fileName))
            return NULL;

        wxFileOffset size = file.Length();

        if (size < 3)
        {
            file.Close();
            return NULL;
        }

        wxByte* buffer = (wxByte*)malloc(sizeof(wxByte) * (size + 1));
        if (!buffer)
        {
            file.Close();
            return NULL;
        }

        size_t readBytes = file.Read((void*)buffer, size);

        assert(readBytes == size);

        Object* object = 0;

        if (strncmp((const char*)buffer, "BIN", 3) == 0)
        {
            std::stringstream stream((const char*)buffer);
            BinVisitor visitror;
            visitror.stream = &stream;
            visitror.action = BinVisitor::VA_Load;
            visitror.visitObject("Object", object, 0, 0);
        }
        else
        {
            TiXmlDocument doc;
            doc.Parse((const char*)buffer);
            XmlVisitor visitror;
            visitror.node = &doc;
            visitror.action = XmlVisitor::VA_Load;
            visitror.visitObject("Object", object, 0, 0);
        }

        free(buffer);
        file.Close();

        return object;
    }

    bool Editor_saveToXml(Object* object, const wxString& fileName)
    {
        if (!object)
            return false;

        wxFile file;
        if (!file.Open(fileName, wxFile::write))
            return false;

        String old_url = object->getUrl();
        object->setUrl(StringUtil::EMPTY);

        TiXmlDocument doc;
        TiXmlDeclaration dec("1.0", "utf-8", "yes");
        doc.InsertEndChild(dec);

        XmlVisitor visitor;
        visitor.node = &doc;
        visitor.action = XmlVisitor::VA_Save;

        visitor.visitObject("Object", object, object->getClassInfo(), 0);

        TiXmlPrinter printer;
        doc.Accept(&printer);

        file.Write((void*)printer.CStr(), printer.Size());
        file.Close();

        object->setUrl(old_url);

        return true;
    }

    bool Editor_saveToBin(Object* object, const wxString& fileName)
    {
        if (!object)
            return false;

        wxFile file;
        if (!file.Open(fileName, wxFile::write))
            return false;

        String old_url = object->getUrl();
        object->setUrl(StringUtil::EMPTY);

        std::stringstream stream;

        stream.write("BIN", 3);

        BinVisitor visitor;
        visitor.stream = &stream;
        visitor.action = BinVisitor::VA_Save;

        visitor.visitObject(object->getClassInfo()->className, object, object->getClassInfo(), 0);

        size_t size = stream.tellp();
        stream.seekg(0);

        file.Write((void*)stream.str().c_str(), size);
        file.Close();

        object->setUrl(old_url);

        return true;
    }

    HARE_ENUM_BEGIN(AppConfigFile::FoldIndicator)
        HARE_ENUM_NAME_VALUE(Arrow, AppConfigFile::FI_Arrow)
        HARE_ENUM_NAME_VALUE(Circle, AppConfigFile::FI_Circle)
        HARE_ENUM_NAME_VALUE(Square, AppConfigFile::FI_Square)
        HARE_ENUM_NAME_VALUE(Simple, AppConfigFile::FI_Simple)
    HARE_ENUM_END()


    HARE_IMPLEMENT_DYNAMIC_CLASS(AppConfigFile, Object, 0)
    {
        HARE_META_F(openFilePath, String, propReadOnly)
        HARE_META_F(openFileFilters, String, propReadOnly)
        HARE_META_F(openFileFilterIndex, int8, propHide)
        HARE_META_F(font, String, propReadOnly)
        HARE_META_F(defaultEncoding, String, propReadOnly)
        HARE_ENUM(show_folds, uint8, Bool)
        HARE_ENUM(fold_xml, uint8, Bool)
        HARE_ENUM(fold_comments, uint8, Bool)
        HARE_ENUM(fold_preprocessor, uint8, Bool)
        HARE_ENUM(fold_indicator, uint8, FoldIndicator)
        HARE_META_F(layout, String, propReadOnly)
        HARE_META_F(language, String, propReadOnly)
    }

    AppConfigFile::AppConfigFile()
        : openFilePath("."), openFileFilters("*.*"), openFileFilterIndex(0),
        show_folds(true), fold_xml(true), fold_comments(true), fold_preprocessor(true),
        fold_indicator(FI_Square), language("en_US")
    {
        defaultEncoding = wxLocale::GetSystemEncodingName().ToUTF8().data();
        font = wxSystemSettings::GetFont(wxSYS_SYSTEM_FIXED_FONT).GetNativeFontInfoDesc().ToUTF8().data();
    }

    wxString AppConfigFile::getOpenFilePath()
    {
        return wxString::FromUTF8(openFilePath.c_str());
    }

    wxString AppConfigFile::getOpenFileFilters()
    {
        return wxString::FromUTF8(openFileFilters.c_str());
    }

    wxString AppConfigFile::getDefaultEncoding()
    {
        return wxString::FromUTF8(defaultEncoding.c_str());
    }

    wxString AppConfigFile::getFont()
    {
        return wxString::FromUTF8(font.c_str());
    }

    wxString AppConfigFile::getLayout()
    {
        return wxString::FromUTF8(layout.c_str());
    }

    wxString AppConfigFile::getLanguage()
    {
        return wxString::FromUTF8(language.c_str());
    }

    int AppConfigFile::getOpenFileFilterIndex()
    {
        return openFileFilterIndex;
    }

    void AppConfigFile::setOpenFilePath(const wxString& value)
    {
        openFilePath = value.ToUTF8().data();
    }

    void AppConfigFile::setOpenFileFilters(const wxString& value)
    {
        openFileFilters = value.ToUTF8().data();
    }

    void AppConfigFile::setOpenFileFilterIndex(int value)
    {
        openFileFilterIndex = value;
    }

    void AppConfigFile::setLayout(const wxString& value)
    {
        layout = value.ToUTF8().data();
    }

    template<> ConfigManager* TManager<ConfigManager>::instance = 0;
    template<> bool TManager<ConfigManager>::autoCreate = true;

    ConfigManager::ConfigManager()
    {
        wxString fileName = Manager::getInstancePtr()->convToEditorDataDir(wxT("app_config.xml"));
        appConfigFile = (AppConfigFile*)Editor_importObject(fileName);
        if (!appConfigFile)
            appConfigFile = new AppConfigFile;
    }

    ConfigManager::~ConfigManager()
    {
        wxString fileName = Manager::getInstancePtr()->convToEditorDataDir(wxT("app_config.xml"));
        Editor_saveToXml(appConfigFile, fileName);
    }
}
