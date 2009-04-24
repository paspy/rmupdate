///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
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
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


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
		wxToolBar* m_toolBar1;
		wxStaticText* m_staticTextProjInfo;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCreateProj( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOpenProj( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		FrameProject( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("RMupdateManager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 273,251 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~FrameProject();
	
};

#endif //__frameUI__
