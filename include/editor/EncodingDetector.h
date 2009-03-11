//***************************************************************
//  File:    EncodingDetector.h
//-------------------------------------------------------------
//  This file is from the Code::Blocks IDE
//  licensed under the GNU Lesser General Public License, version 3
//-------------------------------------------------------------
#ifndef _ENCODINGDETECTOR_H_
#define _ENCODINGDETECTOR_H_

#include "EditorPrerequisites.h"
#include <wx/fontenc.h>
#include <wx/string.h>

namespace hare_editor
{
    class EncodingDetector
    {
    public:
        EncodingDetector(const wxString& filename);
        EncodingDetector(const wxByte* buffer, size_t size);
        EncodingDetector(const EncodingDetector& rhs);
        ~EncodingDetector();

        /** @return True if file was read, false if not. */
        bool getIsOK() const;
        /** @return True if the file contains a BOM (Byte Order Mark), false if not. */
        bool getUsesBOM() const;
        /** @return The BOM size in bytes. Returns zero if no BOM. */
        int getBOMSize() const;
        /** @return The detected encoding. Currently ISO8859-1 is returned if no BOM is present. */
        wxFontEncoding getFontEncoding() const;
        wxString getwxString() const;
    protected:
        /** @return True if succeeded, false if not (e.g. file didn't exist). */
        bool detectEncoding(const wxString& filename, bool convToWX = true);
        bool detectEncoding(const wxByte* buffer, size_t size, bool convToWX = true);

        bool isOK;
        bool useBOM;
        int sizeBOM;
        wxFontEncoding encoding;
    private:
        wxString convStr;
        bool convTowxString(const wxByte* buffer, size_t size);
        inline bool isUTF8Tail(wxByte b) { return ((b & 0xC0) == 0x80); };
        bool detectUTF8(wxByte* byt, size_t size);
        bool isTextUTF16BE(wxByte *text, size_t size);
        bool isTextUTF16LE(wxByte *text, size_t size);
        bool detectUTF16(wxByte* byt, size_t size);
        bool isTextUTF32BE(wxByte *text, size_t size);
        bool isTextUTF32LE(wxByte *text, size_t size);
        bool detectUTF32(wxByte* byt, size_t size);
    };
}

#endif // ENCODINGDETECTOR_H
