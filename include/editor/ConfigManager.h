//***************************************************************
//  File:    ConfigManager.h
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
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "EditorPrerequisites.h"
#include "TManager.h"
#include <wx/string.h>

namespace hare
{
    class EDITOR_API AppConfigFile : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(AppConfigFile)
    public:

        enum FoldIndicator
        {
            FI_Arrow,
            FI_Circle,
            FI_Square,
            FI_Simple,
        };

		wxString getOpenFilePath();
		wxString getOpenFileFilters();
        wxString getDefaultEncoding();
        wxString getFont();
        wxString getLayout();
        wxString getLanguage();
		int getOpenFileFilterIndex();

		void setOpenFilePath(const wxString& value);
		void setOpenFileFilters(const wxString& value);
		void setOpenFileFilterIndex(int value);
        void setLayout(const wxString& value);

	private:

        String openFilePath;
        String openFileFilters;
        int8 openFileFilterIndex;

        String defaultEncoding;
        String font;

        String layout;

        String language;

    public:
        uint8 show_folds;
        uint8 fold_xml;
        uint8 fold_comments;
        uint8 fold_preprocessor;
        uint8 fold_indicator;
	public:

        AppConfigFile();
    };

    class EDITOR_API ConfigManager : public TManager<ConfigManager>
    {
        friend class TManager<ConfigManager>;

    public:
        AppConfigFile* getAppConfigFile()
        {
            return appConfigFile;
        }

    private:
        ConfigManager();
        ~ConfigManager();

        AppConfigFile::Ptr appConfigFile;
    };


}

#endif
