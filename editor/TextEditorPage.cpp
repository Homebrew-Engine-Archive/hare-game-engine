//***************************************************************
//  File:    TextEditorPage.cpp
//  Data:    11/26/2008
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
#include "TextEditorPage.h"
#include "EncodingDetector.h"
#include "EditorManager.h"
#include "ConfigManager.h"
#include "EditorPageManager.h"
#include "ExplorerManager.h"
#include "ProjectExplorer.h"
#include "PluginManager.h"
#include "DebuggerPlugin.h"
#include "TextEditorStyle.h"
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/fontutil.h>
#include <wx/wxscintilla.h>
#include <wx/fontmap.h>
#include <wx/splash.h>

namespace hare_editor
{
    #define ERROR_MARKER        1
    #define ERROR_STYLE         wxSCI_MARK_SMALLRECT
    #define BOOKMARK_MARKER     2
    #define BOOKMARK_STYLE      wxSCI_MARK_ROUNDRECT
    #define BREAKPOINT_MARKER   3
    #define BREAKPOINT_STYLE    wxSCI_MARK_CIRCLE
    #define DEBUG_MARKER        4
    #define DEBUG_STYLE         wxSCI_MARK_ARROW
    
    #define MARGIN_LINENUMBER   0
    #define MARGIN_SYMBOL       1
    #define MARGIN_FOLDING      2


    bool writeStringToFile(wxFile& f, const wxString& data, wxFontEncoding encoding, bool useBOM)
    {
        const char* mark = 0;
        size_t mark_length = 0;

        if (useBOM)
        {
            switch (encoding)
            {
            case wxFONTENCODING_UTF8:
                mark = "\xEF\xBB\xBF";
                mark_length = 3;
                break;
            case wxFONTENCODING_UTF16BE:
                mark = "\xFE\xFF";
                mark_length = 2;
                break;
            case wxFONTENCODING_UTF16LE:
                mark = "\xFF\xFE";
                mark_length = 2;
                break;
            case wxFONTENCODING_UTF32BE:
                mark = "\x00\x00\xFE\xFF";
                mark_length = 4;
                break;
            case wxFONTENCODING_UTF32LE:
                mark = "\xFF\xFE\x00\x00";
                mark_length = 4;
                break;
            case wxFONTENCODING_SYSTEM:
            default:
                break;
            }

            if (f.Write(mark, mark_length) != mark_length)
                return false;
        }

        if (data.length() == 0)
            return true;

        wxCSConv conv(encoding);
        size_t inlen = data.Length(), outlen = 0;
        wxCharBuffer buff = conv.cWC2MB(data.c_str(), inlen, &outlen);

        if (!buff.data() || outlen <= 0)
        {
            buff = wxConvUTF8.cWC2MB(data.c_str(), inlen, &outlen);

            if (!buff.data() || outlen <= 0)
            {
                wxMessageBox(_("The file could not be saved because it contains characters that can neither be represented in your current code page, nor be converted to UTF-8.\n"),
                    _("Failure"), wxICON_WARNING | wxOK);
                return false;
            }
            else
            {
                wxMessageBox(_("The saved document contained characters\nwhich were illegal in the selected encoding.\n\nThe file's encoding has been changed to UTF-8\nto prevent you from losing data."),
                    _("Encoding Changed"), wxICON_INFORMATION | wxOK);
            }
        }

        return f.Write(buff.data(), outlen) == outlen;
    };


    TextEditorPage::TextEditorPage(wxWindow* parent, const wxString& fullFileName)
        : EditorPage(parent), fileName(fullFileName), editorCtrl(0), isModified(false),
        isOK(false), useBOM(false)
    {
        encoding = wxLocale::GetSystemEncoding();
        boxSizer = new wxBoxSizer(wxVERTICAL);
        editorCtrl = createTextEditor();
        boxSizer->Add(editorCtrl, 1, wxEXPAND);
        SetSizer(boxSizer);
        shortName = wxFileName(fullFileName).GetFullName();
        setTextEditorTitle(shortName);

        TextEditorPage::setDefaultEditorStyle(editorCtrl);

        AppConfigFile* appConfig = Manager::getInstancePtr()->getConfigManager()->getAppConfigFile();
        if (appConfig)
        {
            setFoldingIndicator(appConfig->fold_indicator);
        }

        isOK = open();

        updateLineNumberColWidth();
    }

    void TextEditorPage::setTextEditorTitle(const wxString& str)
    {
        if (isModified)
        {
            setTitle(wxT("*") + str);
        }
        else
        {
            setTitle(str);
        }
    }

    bool TextEditorPage::getIsModified() const
    {
        return isModified || getControl()->GetModify();
    }

    void TextEditorPage::setModified(bool modified)
    {
        if (modified != isModified)
        {
            isModified = modified;
            if (!isModified)
            {
                getControl()->SetSavePoint();
            }
            setTextEditorTitle(shortName);
        }
    }

    wxScintilla* TextEditorPage::createTextEditor()
    {
        editorID = wxNewId();
        wxScintilla* control = new wxScintilla(this, editorID, wxDefaultPosition,
            editorCtrl ? wxDefaultSize : GetSize());
        control->UsePopUp(false);
        control->SetCodePage(wxSCI_CP_UTF8);

        Connect(editorID, -1, wxEVT_SCI_MARGINCLICK,
            (wxObjectEventFunction)(wxEventFunction)(wxScintillaEventFunction)
            &TextEditorPage::onMarginClick);
        Connect(editorID, -1, wxEVT_SCI_UPDATEUI,
            (wxObjectEventFunction)(wxEventFunction)(wxScintillaEventFunction)
            &TextEditorPage::onEditorUpdateUI);
        Connect(editorID, -1, wxEVT_SCI_CHANGE,
            (wxObjectEventFunction)(wxEventFunction)(wxScintillaEventFunction)
            &TextEditorPage::onEditorChange);
        Connect(editorID, -1, wxEVT_SCI_MODIFIED,
            (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
            &TextEditorPage::onEditorModified);

        return control;
    }

    void TextEditorPage::onMarginClick(wxScintillaEvent& event)
    {
        switch (event.GetMargin())
        {
        case MARGIN_SYMBOL:
            {
                int lineYpix = event.GetPosition();
                int line = getControl()->LineFromPosition(lineYpix);

                toggleBreakPoint(line);
                break;
            }
        case MARGIN_FOLDING:
            {
                int lineYpix = event.GetPosition();
                int line = getControl()->LineFromPosition(lineYpix);

                getControl()->ToggleFold(line);
                break;
            }
        }
    }

    void TextEditorPage::toggleBreakPoint(int line)
    {
        bool exist = lineHasMarker(BREAKPOINT_MARKER, line);

        if (projectFile)
        {
            if (exist)
                projectFile->removeBreakPoint(line);
            else
                projectFile->addBreakPoint(line);

            Manager* man = Manager::getInstancePtr();
            ProjectExplorer* pe = man->getExplorerManager()->getProjectExplorer();
            Project* prj = pe->getActiveProject();
            if (prj)
            {
                if (!prj->debuggerName.empty())
                {
                    EditorPlugin* plugin = man->getPluginManager()->findPluginByName(wxString::FromUTF8(prj->debuggerName.c_str()));
                    if (plugin && plugin->getType() == EPT_Debugger)
                    {
                        DebuggerPlugin* debugger = (DebuggerPlugin*)plugin;
                        if (exist)
                            debugger->removeBreakPoint(prj->projectName + "/" + projectFile->fileName, line);
                        else
                            debugger->addBreakPoint(prj->projectName + "/" + projectFile->fileName, line);
                    }
                }
            }
        }

        toggleMarker(BREAKPOINT_MARKER, line);
    }

    bool TextEditorPage::getIsOK() const
    {
        return isOK;
    }

    void TextEditorPage::markLine(int marker, int line)
    {
        if (line == -1)
            getControl()->MarkerDeleteAll(marker);
        else
            getControl()->MarkerAdd(line, marker);
    }

    bool TextEditorPage::lineHasMarker(int marker, int line) const
    {
        if (line == -1)
            line = getControl()->GetCurrentLine();
        return getControl()->MarkerGet(line) & (1 << marker) ? true : false;
    }

    void TextEditorPage::toggleMarker(int marker, int line)
    {
        if (line == -1)
            line = getControl()->GetCurrentLine();
        if (lineHasMarker(marker, line))
            getControl()->MarkerDelete(line, marker);
        else
            getControl()->MarkerAdd(line, marker);
    }

    void TextEditorPage::onEditorUpdateUI(wxScintillaEvent& event)
    {
        if (Manager::getInstancePtr()->getEditorPageManager()->getActivePage() == this)
        {
            EditorEvent e(editorEVT_EDITOR_UPDATE_UI);
            e.editorPage = this;
            Manager::getInstancePtr()->processEvent(e);
        }
    }

    void TextEditorPage::onEditorChange(wxScintillaEvent& event)
    {
        setModified(getControl()->GetModify());
    }

    void TextEditorPage::updateLineNumberColWidth()
    {
        int lineNumWidth = 1;
        int lineCount = getControl()->GetLineCount();

        while (lineCount >= 10)
        {
            lineCount /= 10;
            ++lineNumWidth;
        }

        int pixelWidth = getControl()->TextWidth(wxSCI_STYLE_LINENUMBER, _T("9"));
        getControl()->SetMarginWidth(MARGIN_LINENUMBER, 6 + lineNumWidth * pixelWidth);
    }

    void TextEditorPage::onEditorModified(wxScintillaEvent& event)
    {
        int linesAdded = event.GetLinesAdded();
        bool isAdd = event.GetModificationType() & wxSCI_MOD_INSERTTEXT ? true : false;
        bool isDel = event.GetModificationType() & wxSCI_MOD_DELETETEXT ? true : false;
        if ((isAdd || isDel) && linesAdded != 0)
        {
            updateLineNumberColWidth();
        }
    }

    const wxString& TextEditorPage::getFileName()
    {
        return fileName;
    }

    wxString TextEditorPage::getEncodingName()
    {
        return wxFontMapper::GetEncodingName(encoding);
    }

    wxScintilla* TextEditorPage::getControl() const
    {
        return editorCtrl;
    }

    bool TextEditorPage::open(bool detectEncoding)
    {
        if (!wxFileExists(fileName))
            return false;

        editorCtrl->SetReadOnly(false);
        editorCtrl->ClearAll();
        editorCtrl->SetModEventMask(0);

        EncodingDetector enc(fileName);

        if (enc.getIsOK())
        {
            encoding = enc.getFontEncoding();
            useBOM = enc.getUsesBOM();

            editorCtrl->InsertText(0, enc.getwxString());
            editorCtrl->EmptyUndoBuffer();
            editorCtrl->SetModEventMask(wxSCI_MODEVENTMASKALL);

            TextEditorStyle* style = Manager::getInstancePtr()->getEditorPageManager()->getTextEditorStyle();
            style->apply(this, HL_AUTO);

            wxFileName fname(fileName);
            lastModified = fname.GetModificationTime();

            setModified(false);

            return true;
        }

        return false;
    }

    void TextEditorPage::setMarkerStyle(int marker, int markerType, wxColor fore, wxColor back)
    {
        editorCtrl->MarkerDefine(marker, markerType);
        editorCtrl->MarkerSetForeground(marker, fore);
        editorCtrl->MarkerSetBackground(marker, back);
    }

    void TextEditorPage::setFoldingIndicator(int id)
    {
        //Arrow
        if(id == 0)
        {
            setMarkerStyle(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_ARROWDOWN, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_ARROW, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_ARROW, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_ARROWDOWN, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        }

        //Circle
        else if(id == 1)
        {
            setMarkerStyle(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_CIRCLEMINUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_CIRCLEPLUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_VLINE, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_LCORNERCURVE, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_CIRCLEPLUSCONNECTED, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_CIRCLEMINUSCONNECTED, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_TCORNER, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        }

        //Square
        else if(id == 2)
        {
            setMarkerStyle(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_BOXMINUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_BOXPLUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_VLINE, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_LCORNER, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_BOXPLUSCONNECTED, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_BOXMINUSCONNECTED, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_TCORNER, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        }

        //Simple
        else if(id == 3)
        {
            setMarkerStyle(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_MINUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_PLUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_PLUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_MINUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
            setMarkerStyle(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        }
    }

    void TextEditorPage::setDefaultEditorStyle(wxScintilla* control)
    {
        if (!control)
            return;

        AppConfigFile* appConfig = Manager::getInstancePtr()->getConfigManager()->getAppConfigFile();

        if (appConfig)
        {
            wxFont font(10, wxMODERN, wxNORMAL, wxNORMAL);

            wxString fontString = appConfig->getFont();

            if (!fontString.IsEmpty())
            {
                wxNativeFontInfo nfi;
                nfi.FromString(fontString);
                font.SetNativeFontInfo(nfi);
            }

            control->SetCaretLineVisible(true);
            control->SetCaretLineBackground(wxColour(0x80, 0xFF, 0xFF));

            control->SetUseTabs(false);
            control->SetIndentationGuides(false);
            control->SetTabIndents(true);
            control->StyleSetFont(wxSCI_STYLE_DEFAULT, font);
            control->StyleClearAll();
            control->SetTabWidth(4);

            // symbol margin
            control->SetMarginType(MARGIN_SYMBOL, wxSCI_MARGIN_SYMBOL);
            control->SetMarginWidth(MARGIN_SYMBOL, 16);
            control->SetMarginSensitive(MARGIN_SYMBOL, true);
            control->SetMarginMask(MARGIN_SYMBOL, (1 << BOOKMARK_MARKER) | (1 << BREAKPOINT_MARKER) |
                (1 << DEBUG_MARKER) | (1 << ERROR_MARKER));
            control->MarkerDefine(BOOKMARK_MARKER, BOOKMARK_STYLE);
            control->MarkerSetBackground(BOOKMARK_MARKER, wxColour(0xA0, 0xA0, 0xFF));
            control->MarkerDefine(BREAKPOINT_MARKER, BREAKPOINT_STYLE);
            control->MarkerSetBackground(BREAKPOINT_MARKER, wxColour(0xFF, 0x00, 0x00));
            control->MarkerDefine(DEBUG_MARKER, DEBUG_STYLE);
            control->MarkerSetBackground(DEBUG_MARKER, wxColour(0xFF, 0xFF, 0x00));
            control->MarkerDefine(ERROR_MARKER, ERROR_STYLE);
            control->MarkerSetBackground(ERROR_MARKER, wxColour(0xFF, 0x00, 0x00));

            // line number margin
            control->SetMarginType(MARGIN_LINENUMBER, wxSCI_MARGIN_NUMBER);

            // folding margin
            control->SetProperty(wxT("fold"), appConfig->show_folds ? wxT("1") : wxT("0"));
            control->SetProperty(wxT("fold.html"), appConfig->fold_xml ? wxT("1") : wxT("0"));
            control->SetProperty(wxT("fold.comment"), appConfig->fold_comments ? wxT("1") : wxT("0"));
            control->SetProperty(wxT("fold.compact"), wxT("0"));
            control->SetProperty(wxT("fold.preprocessor"), appConfig->fold_preprocessor ?  wxT("1") : wxT("0"));
            if (appConfig->show_folds)
            {
                control->SetFoldFlags(16);
                control->SetMarginType(MARGIN_FOLDING, wxSCI_MARGIN_SYMBOL);
                control->SetMarginWidth(MARGIN_FOLDING, 12);
                control->SetMarginMask(MARGIN_FOLDING, wxSCI_MASK_FOLDERS);
                control->SetMarginSensitive(MARGIN_FOLDING, true);
           }
           else
                control->SetMarginWidth(MARGIN_FOLDING, 0);
        }
    }

    void TextEditorPage::gotoLine(int line)
    {
        int onScreen = getControl()->LinesOnScreen() >> 1;
        getControl()->GotoLine(line - onScreen);
        getControl()->GotoLine(line + onScreen);
        getControl()->GotoLine(line);
        unfoldLine(line);
    }

    void TextEditorPage::setDebugLine(int line)
    {
        markLine(DEBUG_MARKER, line);
    }

    bool TextEditorPage::save()
    {
        if (!getIsModified())
            return true;

        if (!isOK)
        {
            return saveAs();
        }

        wxFile file(fileName.c_str(), wxFile::write);
        if (!file.IsOpened())
            return false;

        if (!writeStringToFile(file, getControl()->GetText(), encoding, useBOM))
        {
            wxMessageBox(wxString::Format(wxT("File %s could not be saved..."), getFileName().c_str()),
                wxT("Error saving file"), wxICON_ERROR);
            return false;
        }

        wxFileName fname(fileName);
        lastModified = fname.GetModificationTime();

        isOK = true;
        getControl()->SetSavePoint();
        setModified(false);

        return true;
    }

    bool TextEditorPage::saveAs()
    {
        wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
            wxT("Save file"));
        if (dlg->ShowModal() != wxID_OK)
        {
            return false;
        }
        fileName = dlg->GetPath();
        wxFileName fname(fileName);
        shortName = fname.GetFullName();
        setTextEditorTitle(shortName);
        isOK = true;
        setModified(true);
        TextEditorStyle* style = Manager::getInstancePtr()->getEditorPageManager()->getTextEditorStyle();
        style->apply(this, HL_AUTO);
        dlg->Destroy();
        return save();
    }

    void TextEditorPage::foldLine(int line)
    {

    }

    void TextEditorPage::unfoldLine(int line)
    {

    }

    void TextEditorPage::undo()
    {
        getControl()->Undo();
    }

    void TextEditorPage::redo()
    {
        getControl()->Redo();
    }

    void TextEditorPage::cut()
    {
        getControl()->Cut();
    }

    void TextEditorPage::copy()
    {
        getControl()->Copy();
    }

    void TextEditorPage::paste()
    {
        getControl()->Paste();
    }

    void TextEditorPage::selectAll()
    {
        getControl()->SelectAll();
    }

    bool TextEditorPage::canUndo() const
    {
        return !isReadOnly() && getControl()->CanUndo();
    }

    bool TextEditorPage::canRedo() const
    {
        return !isReadOnly() && getControl()->CanRedo();
    }

    bool TextEditorPage::canPaste() const
    {
        return !isReadOnly() && getControl()->CanPaste();
    }

    bool TextEditorPage::hasSelection() const
    {
        return getControl()->GetSelectionStart() != getControl()->GetSelectionEnd();
    }

    bool TextEditorPage::isReadOnly() const
    {
        return getControl()->GetReadOnly();
    }

    void TextEditorPage::setProjectFile(ProjectFile* prjFile)
    {
        if (projectFile == prjFile)
            return;

        projectFile = prjFile;

        if (projectFile)
        {
            getControl()->GotoPos(projectFile->editorPos);
            getControl()->ScrollToLine(projectFile->editorTopLine);
            getControl()->ScrollToColumn(0);

            for (size_t i = 0; i < projectFile->breakPoints.size(); ++i)
            {
                markLine(BREAKPOINT_MARKER, projectFile->breakPoints[i]);
            }

            projectFile->editorOpen = true;
            fileName = projectFile->fname.GetFullPath();
            shortName = projectFile->fname.GetFullName();
            setTextEditorTitle(shortName);
        }
    }
    ProjectFile* TextEditorPage::getProjectFile()
    {
        return projectFile;
    }

}
