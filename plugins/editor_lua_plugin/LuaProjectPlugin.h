//***************************************************************
//  File:    LuaProjectPlugin.h
//  Data:    01/06/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef _LUAPROJECTPLUGIN_H_
#define _LUAPROJECTPLUGIN_H_

class FileConfig : Object
{
public:
    String relativePath;
}

class FolderConfig : public Object
{
public:
    String name;
    String filter;
    FolderConfig::Array folders;
    FileConfig::Array files;
}

class ProjectConfig : public Object
{
public:
    String name;
    FolderConfig::Array folders;

}

class LuaProjectPlugin : public Project
{
public:
    /** Add a folder in the project tree.
    * @param name The name of the floder.
    * @param filter The file filter for this folder. @see addFile() for info.
    * @param parent Where to add this floder.
    */
    wxTreeItemId addFolder(const wxString& name, const wxStringArray& filter, const wxTreeItemId& parent);

    bool removeFolder(const wxTreeItemId& folder);

    //ProjectFile* addFile(const wxString& filename, const wxTreeItemId& folder = wxTreeItemId());

    //bool removeFile(const wxString& filename);
    //bool removeFile(int index);


    ///** @return True if the project is modified in any way. */
    //bool isModified() const;

    ///** Mark the project as modified or not.
    //* @param value If true, the project is marked as modified. If false, as not-modified.
    //*/
    //void setModified(bool value = true);

    ///** Close all project files.
    //* @param dontsave If true, no project file will be saved even if modified.
    //* If false, any modified file will be saved (default).
    //* @return True if succesfull, false otherwise.
    //*/
    //bool closeAllFiles(bool dontsave = false);

    ///** Save all project files.
    //* @return True if succesfull, false otherwise.
    //*/
    //bool saveAllFiles();

    ///** Save the project.
    //* @return True if succesfull, false otherwise.
    //*/
    //bool save();

    ///** Save the project under a different name.
    //* A dialog pops up for the user to choose a new filename for the project.
    //* @return True if succesfull, false otherwise.
    //*/
    //bool saveAs();

    ///** Save the project's layout.
    //* Layout is the list of open project files, which one is active,
    //* where the cursor is located on each one of those, etc.
    //* @return True if succesfull, false otherwise.
    //*/
    //bool saveLayout();

    ///** Load the project's layout.
    //* @see saveLayout() for info.
    //* @return True if succesfull, false otherwise.
    //*/
    //bool loadLayout();
};

#endif
