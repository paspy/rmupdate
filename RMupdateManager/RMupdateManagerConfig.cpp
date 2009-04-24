/***************************************************************
 * Name:      RMupdateManagerConfig.cpp
 * Purpose:   Code for Application Frame
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-04-24
 * Copyright: gs (http://www.gsea.com.cn/)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "RMupdateManagerConfig.h"

DECLARE_APP(RMupdateManagerApp);

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


RMupdateManagerConfig::RMupdateManagerConfig(wxFrame *frame)
    : FrameConfig(frame)
{
    proj_info_t proj = wxGetApp().GetProjInfo();
    this->m_textCtrlProjName->SetValue(proj.name);
}

RMupdateManagerConfig::~RMupdateManagerConfig()
{
}

void RMupdateManagerConfig::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void RMupdateManagerConfig::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void RMupdateManagerConfig::SetStatus(wxString& info)
{
    char timestr[100];
    time_t tt;
    struct tm* ttime;

    time(&tt);
    ttime = localtime(&tt);
    strftime(timestr, 100, "\t%H时%M分%S秒", ttime);
    this->m_statusBar->SetStatusText(info + wxString(timestr, wxConvUTF8));

}



