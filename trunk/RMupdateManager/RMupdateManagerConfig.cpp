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
#include <wx/dir.h>
#include "lib/md5.h"

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
        wxbuild << _T("-MacOpenGL");
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
    //变量初始化
    this->MappingChanged = false;
    this->SrcFilesList = new fileinfo_t;
    this->DesFilesList = new fileinfo_t;

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
        m_gridMapping->SetReadOnly(j, 0, true);
        m_gridMapping->SetReadOnly(j, 2, true);
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
        m_gridMapping->SetReadOnly(j, 0, true);
        m_gridMapping->SetReadOnly(j, 2, true);
        j++;
    }

    m_gridMapping->AutoSizeColumns(true);

}

RMupdateManagerConfig::~RMupdateManagerConfig()
{
    delete this->SrcFilesList;
    delete this->DesFilesList;
}

void RMupdateManagerConfig::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void RMupdateManagerConfig::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void RMupdateManagerConfig::SetStatus(wxString info)
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

    //添加到列表
    int cnt = this->m_gridMapping->GetNumberRows();
    this->m_gridMapping->InsertRows(cnt, 1);
    this->m_gridMapping->SetCellValue(cnt, 0, SrcPath);
    this->m_gridMapping->SetCellValue(cnt, 1, DesPath);
    this->m_gridMapping->SetCellValue(cnt, 2, _T("目录"));

    this->MappingChanged = true;
    this->m_buttonCheckUpdate->Enable(true);

    m_gridMapping->AutoSizeColumns(true);
}

void RMupdateManagerConfig::OnAddFile(wxCommandEvent& event)
{
    wxString SrcPath;
    wxString DesPath;

    //选择源路径
    wxFileDialog dlg(NULL);
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }
    else {
        SrcPath = dlg.GetPath();
    }

    //选择目标路径
    /*
    wxFileDialog dlg1(NULL);
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }
    else {
        DesPath = dlg.GetPath();
    }
    */

    //添加到列表
    int cnt = this->m_gridMapping->GetNumberRows();
    this->m_gridMapping->InsertRows(cnt, 1);
    this->m_gridMapping->SetCellValue(cnt, 0, SrcPath);
    this->m_gridMapping->SetCellValue(cnt, 1, DesPath);
    this->m_gridMapping->SetCellValue(cnt, 2, _T("文件"));
    m_gridMapping->SetReadOnly(cnt, 0, true);
    m_gridMapping->SetReadOnly(cnt, 2, true);

    this->MappingChanged = true;
    this->m_buttonCheckUpdate->Enable(true);

    m_gridMapping->AutoSizeColumns(true);
}

void RMupdateManagerConfig::OnCheckUpdate(wxCommandEvent& event)
{
    LoadFilesList();
    /*
    char abc[1024];
    sprintf(abc, "%d", SrcFilesList->md5.Count());
    */
    long UpdateNum = CompareFilesList(SrcFilesList, DesFilesList);
    if (UpdateNum > 0) {
        m_buttonRelease->Enable(true);
        char info[100];
        sprintf(info, "共有%ld个文件更新", UpdateNum);
        SetStatus(wxString(info, wxConvUTF8));
    }
    else {
        SetStatus(_T("没有更新"));
    }
}

void RMupdateManagerConfig::OnRelease(wxCommandEvent& event)
{
    proj_info_t proj = wxGetApp().GetProjInfo();

    m_buttonRelease->Enable(false);

    proj.MappingDirs.SrcPath.Clear();
    proj.MappingDirs.DesPath.Clear();
    proj.MappingFiles.SrcPath.Clear();
    proj.MappingFiles.DesPath.Clear();

    //设置目录映射
    int i;
    for (i = 0; i < m_gridMapping->GetNumberRows(); i++) {
        if (m_gridMapping->GetCellValue(i, 2) == _T("目录")) {
            proj.MappingDirs.SrcPath.Add(m_gridMapping->GetCellValue(i, 0));
            proj.MappingDirs.DesPath.Add(m_gridMapping->GetCellValue(i, 1));
        }
        else {
            proj.MappingFiles.SrcPath.Add(m_gridMapping->GetCellValue(i, 0));
            proj.MappingFiles.DesPath.Add(m_gridMapping->GetCellValue(i, 1));
        }
    }

    wxGetApp().SetProjInfo(proj);
    if (wxGetApp().SaveProject()) {
        SetStatus(_T("保存文件成功"));
    }
    else {
        SetStatus(_T("保存文件失败"));
    }
}

void RMupdateManagerConfig::OnTextChange(wxCommandEvent& event)
{
    proj_info_t proj = wxGetApp().GetProjInfo();
    proj.name = m_textCtrlProjName->GetValue();
    wxGetApp().SetProjInfo(proj);

    m_gridMapping->AutoSizeColumns(true);
}

bool RMupdateManagerConfig::LoadFilesList()
{
    fileinfo_t* list;
    int i;

    list = this->SrcFilesList;
    list->DesPath.Clear();
    list->SrcPath.Clear();
    list->md5.Clear();
    list->size.Clear();

    for (i = 0; i < this->m_gridMapping->GetNumberRows(); i++) {
        if (this->m_gridMapping->GetCellValue(i, 2) == wxString("目录", wxConvUTF8)) {
            char tmp[1024];
            strcpy(tmp, this->m_gridMapping->GetCellValue(i, 0).mb_str());
            printf("进入映射条目：%s\n", tmp);
            this->LoadFolderFiles2List(list, this->m_gridMapping->GetCellValue(i, 0), this->m_gridMapping->GetCellValue(i, 1));
        }
        else {
            this->LoadFile2List(list, this->m_gridMapping->GetCellValue(i, 0), this->m_gridMapping->GetCellValue(i, 1));
        }
    }

    SetStatus(wxString("更新成功", wxConvUTF8));
    return true;
}

bool RMupdateManagerConfig::LoadFolderFiles2List(fileinfo_t*& list, wxString SrcPath, wxString DesPath)
{
    unsigned int i;
    wxDir dir;
    wxArrayString paths;

    //然后遍历文件
    dir.GetAllFiles(SrcPath, &paths, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
    for (i = 0; i < paths.Count(); i++) {
        //这里得到的paths[i]是绝对路径
        wxString DesPathIn = paths[i].Mid(SrcPath.Length() + 1, wxSTRING_MAXLEN);
        LoadFile2List(list, paths[i], DesPathIn);
    }

    return true;
}
bool RMupdateManagerConfig::LoadFile2List(fileinfo_t*& list, wxString SrcPath, wxString DesPath)
{
    char path[1024];
    char md5[32];
    FILE* fp;
    long size;
    void* buffer;

    strcpy(path, SrcPath.mb_str());

    fp = fopen(path, "rb");
    if (!fp) {
        printf("error: can not open %s\n", path );
        SetStatus(wxString("无法打开文件：", wxConvUTF8) + SrcPath);
        return false;
    }

    this->SetStatus(wxString("正在载入文件：", wxConvUTF8) + SrcPath);
    this->Refresh(false);

    #ifdef DEBUG
    char tmp[1024];
    strcpy(tmp, SrcPath.mb_str());
    printf("计算哈希值：%s\n", tmp);
    strcpy(tmp, DesPath.mb_str());
    printf("DesPath: %s\n", tmp);
    #endif

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(size);
    fread(buffer, size, 1, fp);
    fclose(fp);
    md5hash(buffer, size, md5);

    list->size.Add(size);
    list->SrcPath.Add(SrcPath);
    list->DesPath.Add(DesPath);
    list->md5.Add(wxString(md5, wxConvUTF8));

    free(buffer);

    return true;
}

WX_DEFINE_ARRAY_CHAR(char, char_a);
long RMupdateManagerConfig::CompareFilesList(fileinfo_t*& src, fileinfo_t*& des)
{
    /*struct exist_flag_t{
        wxArrayString md5;
        char_a exist;
    } ExistFlag;*/
    unsigned int UnflagSrcNum = src->md5.Count();
    unsigned int UnflagDesNum = des->md5.Count();
    unsigned int ModifiedNum = 0;

    //为ExistFlag载入源列表
    unsigned int i;
    /*for (i = 0; i < UnflagSrcNum; i++) {
        ExistFlag.md5[i] = src->md5[i];
        ExistFlag.exist[i] = 0;
    }*/

    //依次比较des和src的相同的DesPath的文件的哈希值，如果不同，则是修改数+1
    for (i = 0; i < des->md5.Count(); i++) {
        unsigned int k;
        for (k = 0; k < src->DesPath.GetCount(); k++) {
            if (src->DesPath[k] == des->DesPath[k]) break;
        }
        if (k != src->DesPath.GetCount()) {
            //找到了相对路径相同的文件
            UnflagSrcNum--;
            UnflagDesNum--;
            if (src->md5[k] != des->md5[k]) ModifiedNum++;
        }
    }

    return UnflagSrcNum + UnflagDesNum + ModifiedNum;
}




