/***************************************************************
 * Name:      RMupdateManagerMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-04-23
 * Copyright: gs (http://www.gsea.com.cn/)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "RMupdateManagerMain.h"

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


RMupdateManagerFrame::RMupdateManagerFrame(wxFrame *frame)
    : FrameProject(frame)
{

}

RMupdateManagerFrame::~RMupdateManagerFrame()
{
}

void RMupdateManagerFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void RMupdateManagerFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void RMupdateManagerFrame::OnAbout(wxCommandEvent &event)
{

}
