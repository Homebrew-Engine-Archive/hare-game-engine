// ***************************************************************
//  File:   GE_exception.h
//  Date:   2008-03-03 19:02
//  Author: littlesome
//  -------------------------------------------------------------
//
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "CorePrerequisites.h"
#include <exception>

namespace hare
{
    class CORE_API Exception : public std::exception
    {
    protected:
        long line;
        int number;
        String typeName;
        String description;
        String source;
        String file;
        mutable String fullDesc;
    public:
        /** Static definitions of error codes.
        @todo
            Add many more exception codes, since we want the user to be able
            to catch most of them.
        */
        enum ExceptionCodes
        {
            ERR_NOT_IMPLEMENTED,
            ERR_INVALID_STATE,
            ERR_INVALID_PARAMETER,
            ERR_FILE_NOT_FOUND,
            ERR_INTERNAL_ERROR,
        };
        /** Default constructor.
        */
        Exception(int number, const String& description, const String& source);

        /** Advanced constructor.
        */
        Exception(int number, const String& description, const String& source, const char* type, const char* file, long line);

        /** Copy constructor.
        */
        Exception(const Exception& rhs);

        /// Needed for compatibility with std::exception
        ~Exception() throw() {}

        /** Assignment operator.
        */
        void operator = (const Exception& rhs);

        /** Returns a string with the full description of this error.
        */
        virtual const String& getFullDescription() const;

        /** Gets the error code.
        */
        virtual int getNumber() const throw();

        /** Gets the source function.
        */
        virtual const String &getSource() const { return source; }

        /** Gets source file name.
        */
        virtual const String &getFile() const { return file; }

        /** Gets line number.
        */
        virtual long getLine() const { return line; }

        /** Returns a string with only the 'description' field of this exception. Use
            getFullDescriptionto get a full description of the error including line number,
            error number and what function threw the exception.
        */
        virtual const String &getDescription(void) const { return description; }

        /// Override std::exception::what
        const char* what() const throw() { return getFullDescription().c_str(); }

    };

    class CORE_API NotImplementedException : public Exception
    {
    public:
        NotImplementedException(int number, const String& description, const String& source, const char* file, long line)
            : Exception(number, description, source, "NotImplementedException", file, line) {}
    };
    class CORE_API InvalidStateException : public Exception
    {
    public:
        InvalidStateException(int number, const String& description, const String& source, const char* file, long line)
            : Exception(number, description, source, "InvalidStateException", file, line) {}
    };
    class CORE_API InvalidParamterException : public Exception
    {
    public:
        InvalidParamterException(int number, const String& description, const String& source, const char* file, long line)
            : Exception(number, description, source, "InvalidParamterException", file, line) {}
    };
    class CORE_API FileNotFoundException : public Exception
    {
    public:
        FileNotFoundException(int number, const String& description, const String& source, const char* file, long line)
            : Exception(number, description, source, "FileNotFoundException", file, line) {}
    };

    class CORE_API InternalErrorException : public Exception
    {
    public:
        InternalErrorException(int number, const String& description, const String& source, const char* file, long line)
            : Exception(number, description, source, "InternalErrorException", file, line) {}
    };


    template <int num>
    struct ExceptionCodeType
    {
        enum { number = num };
    };

    class ExceptionFactory
    {
    private:

        ExceptionFactory()
        {}
    public:

        static NotImplementedException create(
            ExceptionCodeType<Exception::ERR_NOT_IMPLEMENTED> code,
            const String& desc,
            const String& src, const char* file, long line)
        {
            return NotImplementedException(code.number, desc, src, file, line);
        }

        static InvalidStateException create(
            ExceptionCodeType<Exception::ERR_INVALID_STATE> code,
            const String& desc,
            const String& src, const char* file, long line)
        {
            return InvalidStateException(code.number, desc, src, file, line);
        }

        static InvalidParamterException create(
            ExceptionCodeType<Exception::ERR_INVALID_PARAMETER> code,
            const String& desc,
            const String& src, const char* file, long line)
        {
            return InvalidParamterException(code.number, desc, src, file, line);
        }

        static FileNotFoundException create(
            ExceptionCodeType<Exception::ERR_FILE_NOT_FOUND> code,
            const String& desc,
            const String& src, const char* file, long line)
        {
            return FileNotFoundException(code.number, desc, src, file, line);
        }

        static InternalErrorException create(
            ExceptionCodeType<Exception::ERR_INTERNAL_ERROR> code,
            const String& desc,
            const String& src, const char* file, long line)
        {
            return InternalErrorException(code.number, desc, src, file, line);
        }



    };
}

#ifndef HARE_EXCEPT
#define HARE_EXCEPT(num, desc, src) throw ExceptionFactory::create(\
    ExceptionCodeType<num>(), desc, src, __FILE__, __LINE__);
#endif

#endif

