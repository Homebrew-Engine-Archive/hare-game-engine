//***************************************************************
//  File:    Command.cpp
//  Data:    05/16/2009
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
#include "Command.h"

namespace hare
{
    //////////////////////////////////////////////////////////////////////////
    HARE_IMPLEMENT_ABSTRACT_CLASS(Command, Object, 0) {}

    Command::Command() : executed(false)
    {
    }

    void Command::execute()
    {
        if (!executed)
        {
            doExecute();
            executed = true;
        }
    }

    void Command::restore()
    {
        if (executed)
        {
            doRestore();
            executed = false;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    CommandProcessor::CommandProcessor() : savePoint(0)
    {
    }

    void CommandProcessor::execute(Command::Ptr command)
    {
        command->execute();
        undoStack.push(command);

        while (!redoStack.empty())
            redoStack.pop();
    }

    void CommandProcessor::undo()
    {
        if (!undoStack.empty())
        {
            Command::Ptr command = undoStack.top();
            undoStack.pop();

            command->restore();
            redoStack.push(command);
        }
    }

    void CommandProcessor::redo()
    {
        if (!redoStack.empty())
        {
            Command::Ptr command = redoStack.top();
            redoStack.pop();

            command->execute();
            undoStack.push(command);
        }
    }

    void CommandProcessor::reset()
    {
        while (!redoStack.empty())
            redoStack.pop();

        while (!undoStack.empty())
            undoStack.pop();

        savePoint = 0;
    }

    bool CommandProcessor::canUndo()
    {
        return (!undoStack.empty());
    }

    bool CommandProcessor::canRedo()
    {
        return (!redoStack.empty());
    }

    void CommandProcessor::setSavePoint()
    {
        savePoint = undoStack.size();
    }

    bool CommandProcessor::isAtSavePoint()
    {
        return undoStack.size() == savePoint;
    }
}

