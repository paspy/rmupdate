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

#include "RMupdateManagerApp.h"
#include "RMupdateManagerMain.h"
#include "RMupdateManagerConfig.h"
#include "RMupdateManagerAbout.h"
DECLARE_APP(RMupdateManagerApp);

extern RMupdateManagerConfig* frameConfig;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };


RMupdateManagerFrame::RMupdateManagerFrame(wxFrame *frame)
    : FrameProject(frame)
{
	SetIcon(wxGetApp().icon);
}

RMupdateManagerFrame::~RMupdateManagerFrame()
{
}

void RMupdateManagerFrame::OnClose(wxCloseEvent &event)
{
	wxGetApp().TryQuit();
}

void RMupdateManagerFrame::OnQuit(wxCommandEvent &event)
{
    wxGetApp().TryQuit();
}

void RMupdateManagerFrame::OnOnlineHelp(wxCommandEvent& event)
{
	OPENLINK("http://code.google.com/p/rmupdate/wiki/RMupdateManager_how_to_use");
}

void RMupdateManagerFrame::OnAbout(wxCommandEvent &event)
{
	RMupdateManagerAbout a((wxFrame*)this, _T("关于"));
	a.ShowModal();
}

void RMupdateManagerFrame::OnCreateProj( wxCommandEvent& event )
{
	if (wxGetApp().GetProjModified()) {
		wxMessageDialog dlg(this, _("打开的工程已经修改，是否关闭当前已经打开的工程并创建新工程？"), wxGetApp().GetAppName(), wxICON_EXCLAMATION | wxYES | wxNO);
		if (dlg.ShowModal() != wxID_OK) {
			return ;
		}
	}

	// 如果当前已经打开了工程，则关闭
	if (wxGetApp().GetProjectOpened()) {
		frameConfig->Destroy();
	}

    if (wxGetApp().CreateProj()) RefreshProjInfo();
}

void RMupdateManagerFrame::OnOpenProj( wxCommandEvent& event )
{
	if (wxGetApp().GetProjModified()) {
		wxMessageDialog dlg(this, _("打开的工程已经修改，是否关闭当前已经打开的工程并打开另一工程？"), wxGetApp().GetAppName(), wxICON_EXCLAMATION | wxYES | wxNO);
		if (dlg.ShowModal() != wxID_OK) {
			return ;
		}
	}

	// 如果当前已经打开了工程，则关闭
	if (wxGetApp().GetProjectOpened()) {
		frameConfig->Destroy();
	}

	// 打开工程并产生窗口
    if (wxGetApp().OpenProj()) {
        frameConfig = new RMupdateManagerConfig(0L);
        frameConfig->Show();
    }
    RefreshProjInfo();
}

void RMupdateManagerFrame::RefreshProjInfo()
{
            //在工程主窗口中显示工程信息
    wxString info;
    proj_info_t proj = wxGetApp().GetProjInfo();

    struct tm *ttime;
    char timestr[100];
    ttime = localtime(&proj.UpdateTime);
    strftime(timestr, 100, "%Y年%m月%d日 %H时%M分%S秒", ttime);

    info.sprintf(_T("工程版本号：%ld.%ld\n映射目录数：%ld\n映射文件数：%ld\n"),
                    proj.AbsVer,
                    proj.SubAbsVer,
                    proj.MappingDirs.SrcPath.Count(),
                    proj.MappingFiles.SrcPath.Count());
    info += _T("最后更新时间：") + wxString(timestr, wxConvLibc);
    info = _T("工程名：") + proj.name + wxT("\n") + info;
    info = _T("工程配置：") + proj.ProjPath + wxT("\n") + info;
    info = _T("发布版本：") + proj.version+ wxT("\n") + info;

    m_staticTextProjInfo->SetLabel(info);

	// 根据工程是否被修改了设置标签颜色
	wxColor color;
    if (wxGetApp().GetProjModified()) {
    	color = wxColor(0, 0, 255);
    }
    else {
    	color = wxColor(0, 0, 0);
    }
	m_staticTextProjInfo->SetForegroundColour(color);
}

void RMupdateManagerFrame::OnSaveProj( wxCommandEvent& event )
{
    wxGetApp().SaveProject();
}

void RMupdateManagerFrame::ProjectOpened(bool isOpened)
{
	if (isOpened) {
	}
	else {
		m_staticTextProjInfo->SetLabel(_("没有打开工程"));
	}
}
