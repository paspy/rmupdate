/***************************************************************
 * Name:      RMupdaterConfig.cpp
 * Purpose:   Code for Application Frame
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-05-01
 * Copyright: GPLv3
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "RMupdaterConfig.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

DECLARE_APP(RMupdaterApp);

RMupdaterConfig::RMupdaterConfig(wxFrame *frame, const wxString& title)
    : DialogConfig(frame, -1, title)
{
	config_t c = wxGetApp().GetConfig();
	if (c.CheckInterval == 7) {
		m_radioBoxUpdateInterval->SetSelection(0);
	}
	else {
		m_radioBoxUpdateInterval->SetSelection(1);
	}
}


RMupdaterConfig::~RMupdaterConfig()
{
}

void RMupdaterConfig::OnOK( wxCommandEvent& event )
{
	config_t c = wxGetApp().GetConfig();

	if (m_radioBoxUpdateInterval->GetSelection() == 0) {
		c.CheckInterval = 7;
	}
	else {
		c.CheckInterval = 0;
	}

	wxGetApp().SetConfig(c);
	wxGetApp().SaveConfig();

	EndModal(wxID_OK);
}
