///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __RMupdaterUI__
#define __RMupdaterUI__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/gauge.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/radiobox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_CHECK 1000
#define wxID_START 1001

///////////////////////////////////////////////////////////////////////////////
/// Class FrameUpdater
///////////////////////////////////////////////////////////////////////////////
class FrameUpdater : public wxFrame 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmapLogo;
		wxRichTextCtrl* m_richTextInfo;
		wxButton* m_buttonCheck;
		wxButton* m_buttonUpdate;
		wxButton* m_buttonConfig;
		wxButton* m_buttonStart;
		wxStaticText* m_staticTextCurProc;
		wxGauge* m_gaugeCurrent;
		wxStaticText* m_staticTextTotalProc;
		wxGauge* m_gaugeTotal;
		wxStatusBar* m_statusBarInfo;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnConfig( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnStart( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		FrameUpdater( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("邀踢小传更新程序"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,358 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~FrameUpdater();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogConfig
///////////////////////////////////////////////////////////////////////////////
class DialogConfig : public wxDialog 
{
	private:
	
	protected:
		wxRadioBox* m_radioBoxUpdateInterval;
		wxButton* m_buttonClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		DialogConfig( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("更新设置"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 127,142 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DialogConfig();
	
};

#endif //__RMupdaterUI__
