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
    //设置界面
    this->m_gridMapping->SetColLabelValue(0, _T("源"));
    this->m_gridMapping->SetColLabelValue(1, _T("目的"));
    this->m_gridMapping->SetColLabelValue(2, _T("类型"));

    //载入工程配置
    proj_info_t proj = wxGetApp().GetProjInfo();
    this->m_textCtrlProjName->SetValue(proj.name);

    //载入映射列表
    unsigned int i, j = 0;
    wxArrayString SrcPath;
    wxArrayString DesPath;

    proj = wxGetApp().GetProjInfo();

    //目录
    SrcPath = proj.MappingDirs.SrcPath;
    DesPath = proj.MappingDirs.DesPath;
    this->m_gridMapping->InsertRows(0, SrcPath.Count());
    for (i = 0; i < SrcPath.Count(); i++) {
        this->m_gridMapping->SetCellValue(j, 0, SrcPath[i]);
        this->m_gridMapping->SetCellValue(j, 1, DesPath[i]);
        this->m_gridMapping->SetCellValue(j, 2, wxString("目录", wxConvUTF8));
        j++;
    }

    //文件
    SrcPath = proj.MappingFiles.SrcPath;
    DesPath = proj.MappingFiles.DesPath;
    this->m_gridMapping->InsertRows(j, SrcPath.Count());
    for (i = 0; i < SrcPath.Count(); i++) {
        this->m_gridMapping->SetCellValue(j, 0, SrcPath[i]);
        this->m_gridMapping->SetCellValue(j, 1, DesPath[i]);
        this->m_gridMapping->SetCellValue(j, 2, wxString("文件", wxConvUTF8));
        j++;
    }

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

void RMupdateManagerConfig::OnAddDir(wxCommandEvent& event)
{
    wxString SrcPath;
    wxString DesPath;

    //选择源路径
    wxDirDialog dlg(NULL);
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }
    else {
        SrcPath = dlg.GetPath();
    }

    //选择目标路径
    wxDirDialog dlg1(NULL);
    if (dlg1.ShowModal() != wxID_OK) {
        return;
    }
    else {
        DesPath = dlg1.GetPath();
    }

    //添加到列表
    int cnt = this->m_gridMapping->GetNumberRows();
    this->m_gridMapping->InsertRows(cnt, 1);
    this->m_gridMapping->SetCellValue(cnt, 0, SrcPath);
    this->m_gridMapping->SetCellValue(cnt, 1, DesPath);
    this->m_gridMapping->SetCellValue(cnt, 2, _T("目录"));

}

void RMupdateManagerConfig::OnAddFile(wxCommandEvent& event)
{
}

void RMupdateManagerConfig::OnCheck(wxCommandEvent& event)
{
}

void RMupdateManagerConfig::OnSave(wxCommandEvent& event)
{
}

void RMupdateManagerConfig::OnRelease(wxCommandEvent& event)
{
}




