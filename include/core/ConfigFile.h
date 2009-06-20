//***************************************************************
//  File:    ConfigFile.h
//  Data:    02/01/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "CorePrerequisites.h"
#include "StringUtil.h"

namespace hare
{
    /** Class for quickly loading settings from a text file.
    */
    class CORE_API ConfigFile
    {
    public:
        ~ConfigFile();

        /** Load from filename (not using game file-system).
        */
        void load(const String& filename, const String& separators = "=");

        /** Clear the settings */
        void clear();

        /** Gets the first setting from the file with the named key. 
        @param 
            key The name of the setting
        @param 
            section The name of the section it must be in (if any)
        */
        String getSetting(const String& key, const String& section = StringUtil::EMPTY) const;

        /** Gets all settings from the file with the named key. 
        */
        StringVector getMultiSetting(const String& key, const String& section = StringUtil::EMPTY) const;

        typedef std::multimap<String, String> SettingsMultiMap;
        typedef std::map<String, SettingsMultiMap*> SettingsBySection;
    protected:
        SettingsBySection settings;
    };
}

#endif
