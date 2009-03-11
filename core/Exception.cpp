// ***************************************************************
//  File:   GE_exception.cpp
//  Date:   2008-03-03 19:02
//  Author: littlesome
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "PCH.h"
#include "Exception.h"
#include <sstream>

namespace hare_core 
{
    Exception::Exception(int num, const String& desc, const String& src) 
        :line(0), number(num), description(desc), source(src)
    {
    }

    Exception::Exception(int num, const String& desc, const String& src, 
        const char* typ, const char* fil, long lin) 
        :line(lin), number(num), typeName(typ), description(desc), source(src), file(fil)
    {
    }

    Exception::Exception(const Exception& rhs)
        : line(rhs.line), number(rhs.number), description(rhs.description), 
        source(rhs.source), file(rhs.file)
    {
    }

    void Exception::operator = (const Exception& rhs)
    {
        description = rhs.description;
        number = rhs.number;
        source = rhs.source;
        file = rhs.file;
        line = rhs.line;
        typeName = rhs.typeName;
    }

    const String& Exception::getFullDescription(void) const
    {
        if (fullDesc.empty())
        {
            std::ostringstream desc;

            desc << "HARE EXCEPTION(" << number << ":" << typeName << "): "
                 << description 
                 << " in " << source;

            if (line > 0)
            {
                desc << " at " << file << " (line " << line << ")";
            }

            fullDesc = desc.str();
        }

        return fullDesc;
    }

    int Exception::getNumber(void) const throw()
    {
        return number;
    }
}









