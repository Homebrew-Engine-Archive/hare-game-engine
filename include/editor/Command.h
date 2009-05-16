//***************************************************************
//  File:    Command.h
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
#include "EditorPrerequisites.h"
#include <stack>

namespace hare
{
    class EDITOR_API Command : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(Command)
    private:
        bool executed;

    protected:
        virtual void doExecute() = 0;
        virtual void doRestore() = 0;

    public:
        Command();
        virtual ~Command() {};

        void execute();
        void restore();
    };

    class EDITOR_API CommandProcessor
    {
    private:
        typedef std::stack<Command::Ptr> CommandStack;

        CommandStack undoStack;
        CommandStack redoStack;
        size_t savePoint;

    public:
        CommandProcessor();
        void execute(Command::Ptr command);

        void undo();
        void redo();
        void reset();

        void setSavePoint();
        bool isAtSavePoint();

        bool canUndo();
        bool canRedo();
    };
}

