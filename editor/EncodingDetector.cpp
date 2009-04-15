//***************************************************************
//  File:    EncodingDetector.cpp
//-------------------------------------------------------------
//  This file is from the Code::Blocks IDE
//  licensed under the GNU Lesser General Public License, version 3
//-------------------------------------------------------------
#include "PCH.h"
#include "EncodingDetector.h"
#include "EditorManager.h"
#include "ConfigManager.h"
#include <wx/fontmap.h>
#include <wx/intl.h>
#include <wx/file.h>


namespace hare
{
    EncodingDetector::EncodingDetector(const wxString& filename)
        : isOK(false), useBOM(false), sizeBOM(0), convStr(wxEmptyString)
    {
        encoding = wxLocale::GetSystemEncoding();
        isOK = detectEncoding(filename);
    }

    EncodingDetector::EncodingDetector(const wxByte* buffer, size_t size)
        : isOK(false), useBOM(false), sizeBOM(0), convStr(wxEmptyString)
    {
        encoding = wxLocale::GetSystemEncoding();
        isOK = detectEncoding(buffer, size);
    }

    EncodingDetector::EncodingDetector(const EncodingDetector& rhs)
        : isOK(rhs.isOK), useBOM(rhs.useBOM), sizeBOM(rhs.sizeBOM),
        encoding(rhs.encoding), convStr(wxEmptyString)
    {
    }

    EncodingDetector::~EncodingDetector()
    {
    }

    bool EncodingDetector::getIsOK() const
    {
        return isOK;
    }

    bool EncodingDetector::getUsesBOM() const
    {
        return useBOM;
    }

    int EncodingDetector::getBOMSize() const
    {
        return sizeBOM;
    }

    wxFontEncoding EncodingDetector::getFontEncoding() const
    {
        return encoding;
    }

    wxString EncodingDetector::getwxString() const
    {
        return convStr;
    }

    bool EncodingDetector::convTowxString(const wxByte* buffer, size_t size)
    {
        if (!buffer || size == 0)
            return false;

        if (sizeBOM > 0)
        {
            for (int i = 0; i < sizeBOM; ++i)
                *buffer++;
        }

        size_t outlen = 0;
        wxCSConv conv(encoding);

        wxWCharBuffer wideBuff = conv.cMB2WC((char*)buffer, size - sizeBOM, &outlen);
        convStr = wxString(wideBuff);

        if (outlen == 0)
        {
            // Possibly the conversion has failed. Let's try with System-default encoding
#if defined(__WXMSW__)
            encoding = wxLocale::GetSystemEncoding();
#else
            // We can rely on the UTF-8 detection code ;-)
            encoding = wxFONTENCODING_ISO8859_1;
#endif
            wxCSConv conv(encoding);
            wxWCharBuffer wideBuff = conv.cMB2WC((char*)buffer, size - sizeBOM, &outlen);
            convStr = wxString(wideBuff);
        }

        if (outlen == 0)
        {
            return false;
        }

        return true;
    }

    bool EncodingDetector::detectEncoding(const wxString& filename, bool convToWX)
    {
        wxFile file(filename);
        if (!file.IsOpened())
            return false;

        size_t size = file.Length();
        if (size == 0)
        {
            file.Close();
            return false;
        }

        wxByte* buffer = (wxByte*)malloc(sizeof(wxByte) * (size + 1));
        if (!buffer)
        {
            file.Close();
            return false;
        }

        size_t readBytes = file.Read((void*)buffer, size);
        bool result = false;
        if (readBytes > 0)
        {
            result = detectEncoding(buffer, size, convToWX);
        }

        file.Close();
        free(buffer);
        return result;
    }

    bool EncodingDetector::detectEncoding(const wxByte* buffer, size_t size, bool convToWX)
    {
        if (!buffer)
            return false;

        if (size >= 4)
        {
            char buff[4] = {'\0'};
            memcpy(buff, buffer, 4);

            if (memcmp(buff, "\xEF\xBB\xBF", 3) == 0)
            {
                useBOM = true;
                sizeBOM = 3;
                encoding = wxFONTENCODING_UTF8;
            }
            else if (memcmp(buff, "\x00\x00\xFE\xFF", 4) == 0)
            {
                useBOM = true;
                sizeBOM = 4;
                encoding = wxFONTENCODING_UTF32BE;
            }
            else if (memcmp(buff, "\xFF\xFE\x00\x00", 4) == 0)
            {
                useBOM = true;
                sizeBOM = 4;
                encoding = wxFONTENCODING_UTF32LE;
            }
            else if (memcmp(buff, "\xFE\xFF", 2) == 0)
            {
                useBOM = true;
                sizeBOM = 2;
                encoding = wxFONTENCODING_UTF16BE;
            }
            else if (memcmp(buff, "\xFF\xFE", 2) == 0)
            {
                useBOM = true;
                sizeBOM = 2;
                encoding = wxFONTENCODING_UTF16LE;
            }
        }

        if (!useBOM)
        {
            if (detectUTF8((wxByte*)buffer, size))
            {
                encoding = wxFONTENCODING_UTF8;
            }
            else if (!detectUTF16((wxByte*)buffer, size) && !detectUTF32((wxByte*)buffer, size))
            {
                // Use user-specified one
                AppConfigFile* appConfig = Manager::getInstancePtr()->getConfigManager()->getAppConfigFile();
                encoding = wxFontMapper::Get()->CharsetToEncoding(appConfig->getDefaultEncoding());
            }

            useBOM = false;
            sizeBOM = 0;
        }

        if (convToWX)
        {
            convTowxString(buffer, size);
        }

        return true;
    } // end of detectEncoding

    bool EncodingDetector::detectUTF8(wxByte *byt, size_t size)
    {
        size_t i = 0;

        if (!byt)
            return false;

        while (i < size)
        {
            if (byt[i] < 0x80)
            {
                ++i; // Continue searching for any possible UTF-8 encoded characters
            }
            else if (byt[i] <= 0xDF) // 1110xxxx 10xxxxxx
            {
                if (++i < size && isUTF8Tail(byt[i]))
                {
                    return true; // We would stop our search assuming the whole file is UTF-8
                }
                else if (i != size) // Possibly a malformed UTF-8 file
                {
                    return false;
                }
            }
            else if (byt[i] <= 0xEF) // 1110xxxx 10xxxxxx 10xxxxxx
            {
                if ((++i < size && isUTF8Tail(byt[i]))
                    && (++i < size && isUTF8Tail(byt[i])))
                {
                    return true; // We would stop our search assuming the whole file is UTF-8
                }
                else if(size != i)
                {
                    return false;
                }
            }
            else if (byt[i] <= 0xF4) // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                if ((++i < size && isUTF8Tail(byt[i]))
                    && (++i < size && isUTF8Tail(byt[i]))
                    && (++i < size && isUTF8Tail(byt[i])))
                {
                    return true;
                }
                else if(size != i)
                {
                    return false;
                }
            }
            else if (byt[i] <= 0xFB)  // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                if ((++i < size && isUTF8Tail(byt[i]))
                    && (++i < size && isUTF8Tail(byt[i]))
                    && (++i < size && isUTF8Tail(byt[i]))
                    && (++i < size && isUTF8Tail(byt[i])))
                {
                    return true;
                }
                else if(size != i)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        return false;
    }

    bool EncodingDetector::isTextUTF16LE(wxByte *text, size_t size)
    {
        if (size < 2)
            return false;

        bool ok = false;
        bool highsurrogate = false;

        size = size & 0x1FFFFFFE;   // to even
        while(size > 0)
        {
            if(text[1] == 0)
            {
                if(text[0] == 0)
                    return false;
                ok = true;
            }
            else if(text[1] >= 0xD8 && text[1] <= 0xDB)
            {
                if(highsurrogate)
                    return false;
                highsurrogate = true;
            }
            else if(text[1] >= 0xDC && text[1] <= 0xDF)
            {
                if(!highsurrogate)
                    return false;
                highsurrogate = false;
            }

            size -= 2;
            text += 2;
        }

        return ok;
    }

    bool EncodingDetector::isTextUTF16BE(wxByte *text, size_t size)
    {
        if (size < 2)
            return false;

        bool ok = false;
        bool highsurrogate = false;

        size = size & 0x1FFFFFFE;     // to even

        while(size > 0)
        {
            if(text[0] == 0)
            {
                if(text[1] == 0)
                    return false;
                ok = true;
            }
            else if(text[0] >= 0xD8 && text[0] <= 0xDB)
            {
                if(highsurrogate)
                    return false;
                highsurrogate = true;
            }
            else if(text[0] >= 0xDC && text[0] <= 0xDF)
            {
                if(!highsurrogate)
                    return false;
                highsurrogate = false;
            }

            size -= 2;
            text += 2;
        }

        return ok;
    }

    bool EncodingDetector::detectUTF16(wxByte *byt, size_t size)
    {
        if (isTextUTF16BE(byt, size))
        {
            encoding = wxFONTENCODING_UTF16BE;
            return true;
        }
        else if (isTextUTF16LE(byt, size))
        {
            encoding = wxFONTENCODING_UTF16LE;
            return true;
        }
        return false;
    }


    bool EncodingDetector::isTextUTF32LE(wxByte *text, size_t size)
    {
        size >>= 2;   // to count
        if (size < 4)
            return false;

        wxUint32 ucs4, *p=(wxUint32 *)text;

        for(size_t i = 0; i < size; i++, p++)
        {
            ucs4 = wxINT32_SWAP_ON_BE(*p);

            if(ucs4 <= 0 || ucs4 > 0x10FFFF)
                return false;
        }

        return true;
    }

    bool EncodingDetector::isTextUTF32BE(wxByte *text, size_t size)
    {
        size >>= 2;   // to count
        if (size < 4)
            return false;

        wxUint32 ucs4, *p = (wxUint32 *)text;

        for (size_t i = 0; i < size; i++, p++)
        {
            ucs4 = wxINT32_SWAP_ON_LE(*p);

            if (ucs4 <= 0 || ucs4 > 0x10FFFF)
                return false;
        }

        return true;
    }

    bool EncodingDetector::detectUTF32(wxByte *byt, size_t size)
    {
        if (isTextUTF32BE(byt, size))
        {
            encoding = wxFONTENCODING_UTF32BE;
            return true;
        }
        else if (isTextUTF32LE(byt, size))
        {
            encoding = wxFONTENCODING_UTF32LE;
            return true;
        }
        return false;
    }
}
