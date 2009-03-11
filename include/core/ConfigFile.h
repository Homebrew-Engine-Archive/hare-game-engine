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

namespace hare_core 
{
    class CORE_API ConfigFile
    {
    public:
        ConfigFile();
        virtual ~ConfigFile();

        void load(const String& filename, const String& separators = "=");
        void clear();

        String getSetting(const String& key, const String& section = StringUtil::EMPTY) const;
        StringVector getMultiSetting(const String& key, const String& section = StringUtil::EMPTY) const;

        typedef std::multimap<String, String> SettingsMultiMap;
        typedef std::map<String, SettingsMultiMap*> SettingsBySection;
    protected:
        SettingsBySection settings;
    };
}

#endif
