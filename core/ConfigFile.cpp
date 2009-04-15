//***************************************************************
//  File:    ConfigFile.cpp
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
#include "PCH.h"
#include "ConfigFile.h"

namespace hare
{
    ConfigFile::ConfigFile()
    {
    }
    ConfigFile::~ConfigFile()
    {
        SettingsBySection::iterator seci, secend;
        secend = settings.end();
        for (seci = settings.begin(); seci != secend; ++seci)
        {
            delete seci->second;
        }
    }

    void ConfigFile::load(const String& filename, const String& separators)
    {
        clear();

        std::ifstream stream;
        stream.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);

        if (!stream)
            return;

        String currentSection = StringUtil::EMPTY;
        SettingsMultiMap* currentSettings = new SettingsMultiMap();
        settings[currentSection] = currentSettings;

        /* Process the file line for line */
        String line, optName, optVal;
        while (!stream.eof())
        {
            std::getline(stream, line);

            if (line.length() > 0 && line.at(0) != '#')
            {
                if (line.at(0) == '[' && line.at(line.length()-1) == ']')
                {
                    currentSection = line.substr(1, line.length() - 2);
                    SettingsBySection::const_iterator seci = settings.find(currentSection);
                    if (seci == settings.end())
                    {
                        currentSettings = new SettingsMultiMap();
                        settings[currentSection] = currentSettings;
                    }
                    else
                    {
                        currentSettings = seci->second;
                    }
                }
                else
                {
                    String::size_type separator_pos = line.find_first_of(separators, 0);
                    if (separator_pos != String::npos)
                    {
                        optName = line.substr(0, separator_pos);
                        String::size_type nonseparator_pos = line.find_first_not_of(separators, separator_pos);
                        optVal = (nonseparator_pos == String::npos) ? "" : line.substr(nonseparator_pos);
                        StringUtil::trim(optVal);
                        StringUtil::trim(optName);
                        currentSettings->insert(std::multimap<String, String>::value_type(optName, optVal));
                    }
                }
            }
        }
    }

    void ConfigFile::clear()
    {
        for (SettingsBySection::iterator seci = settings.begin();
            seci != settings.end(); ++seci)
        {
            delete seci->second;
        }
        settings.clear();
    }

    String ConfigFile::getSetting(const String& key, const String& section) const
    {
        SettingsBySection::const_iterator seci = settings.find(section);
        if (seci == settings.end())
        {
            return StringUtil::EMPTY;
        }
        else
        {
            SettingsMultiMap::const_iterator i = seci->second->find(key);
            if (i == seci->second->end())
            {
                return StringUtil::EMPTY;
            }
            else
            {
                return i->second;
            }
        }
    }

    StringVector ConfigFile::getMultiSetting(const String& key, const String& section) const
    {
        StringVector ret;

        SettingsBySection::const_iterator seci = settings.find(section);
        if (seci != settings.end())
        {
            SettingsMultiMap::const_iterator i;
            i = seci->second->find(key);
            while (i != seci->second->end() && i->first == key)
            {
                ret.push_back(i->second);
                ++i;
            }
        }
        return ret;
    }
}
