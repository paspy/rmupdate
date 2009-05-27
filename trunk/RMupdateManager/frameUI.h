///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __frameUI__
#define __frameUI__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/grid.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/hyperlink.h>
#include <wx/panel.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_ADDFILE 1000
#define wxID_ADDDIR 1001
#define wxID_CHECK 1002
#define wxID_RELEASE 1003

///////////////////////////////////////////////////////////////////////////////
/// Class FrameProject
///////////////////////////////////////////////////////////////////////////////
class FrameProject : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menuFile;
		wxMenu* m_menuHelp;
		wxStaticText* m_staticTextProjInfo;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnCreateProj( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOpenProj( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnSaveProj( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOnlineHelp( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		FrameProject( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("RMupdateManager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 344,194 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~FrameProject();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class FrameConfig
///////////////////////////////////////////////////////////////////////////////
class FrameConfig : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textCtrlProjName;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrlVersion;
		wxGrid* m_gridMapping;
		wxButton* m_buttonAddFile;
		wxButton* m_buttonAddDir;
		wxButton* m_buttonDelete;
		wxButton* m_buttonCheckUpdate;
		wxButton* m_buttonRelease;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnTextChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnGridCellChange( wxGridEvent& event ){ event.Skip(); }
		virtual void OnAddFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAddDir( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDelete( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckUpdate( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRelease( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		FrameConfig( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("工程管理"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 497,342 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~FrameConfig();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogAbout
///////////////////////////////////////////////////////////////////////////////
class DialogAbout : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxNotebook* m_notebookInfo;
		wxPanel* m_panelInfo;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText6;
		wxHyperlinkCtrl* m_hyperlink1;
		wxPanel* m_panelCredits;
		wxRichTextCtrl* m_richTextCredits;
		wxButton* m_buttonOK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnQuit( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		DialogAbout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("关于"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 330,317 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DialogAbout();
	
};

#endif //__frameUI__
