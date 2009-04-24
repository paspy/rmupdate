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

void RMupdateManagerFrame::OnCreateProj( wxCommandEvent& event )
{
}

void RMupdateManagerFrame::OnOpenProj( wxCommandEvent& event )
{
    if (wxGetApp().OpenProj()) {;
        //在工程主窗口中显示工程信息
        wxString info;
        proj_info_t proj = wxGetApp().GetProjInfo();

        info.sprintf(_T("工程版本号：%ld.%ld\n映射目录数：%ld\n映射文件数：%ld\n最后更新时间：%ld"),
                        proj.AbsVer,
                        proj.SubAbsVer,
                        proj.MappingDirs.SrcPath.Count(),
                        proj.MappingFiles.SrcPath.Count(),
                        proj.UpdateTime);
        info = _T("工程名：") + proj.name + wxT("\n") + info;
        m_staticTextProjInfo->SetLabel(info);
    }

}
