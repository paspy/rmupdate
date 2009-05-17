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
#include <wx/file.h>
#include "lib/md5.h"
#include "ticpp/tinyxml.h"

#ifdef RMUPDATE_ENCRYPT_FILE
#include "lib/file_encrypt.h"
#endif


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
    m_textCtrlProjName->SetValue(proj.name);
    m_textCtrlVersion->SetValue(proj.version);

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
        this->m_gridMapping->SetCellValue(j, 2, _T("目录"));
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
        this->m_gridMapping->SetCellValue(j, 2, _T("文件"));
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
    this->m_statusBar->SetStatusText(info + wxString(timestr, wxConvLibc));
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
        SetStatus(wxString(info, wxConvLibc));
    }
    else {
        SetStatus(_T("没有更新"));
    }

    //m_buttonCheckUpdate->Enable(false);
}

void RMupdateManagerConfig::OnRelease(wxCommandEvent& event)
{
    proj_info_t proj = wxGetApp().GetProjInfo();

    //m_buttonRelease->Enable(false);

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
    wxGetApp().SaveProject();

	UpdateResourceFiles();
    SaveFilesList();
    UpdateUpdateFile();
}

void RMupdateManagerConfig::OnTextChange(wxCommandEvent& event)
{
    proj_info_t proj = wxGetApp().GetProjInfo();

    if (m_textCtrlProjName->GetValue() != wxT("")) proj.name = m_textCtrlProjName->GetValue();
    if (m_textCtrlVersion->GetValue() != wxT("")) proj.version = m_textCtrlVersion->GetValue();

    wxGetApp().SetProjInfo(proj);
}

bool RMupdateManagerConfig::LoadFilesList()
{
    fileinfo_t* list;
    int i;

    //载入目的文件表
    list = this->DesFilesList;
    list->DesPath.Clear();
    list->SrcPath.Clear();
    list->md5.Clear();
    list->size.Clear();

    for (i = 0; i < this->m_gridMapping->GetNumberRows(); i++) {
        if (this->m_gridMapping->GetCellValue(i, 2) == _T("目录")) {
            char tmp[1024];
            strcpy(tmp, this->m_gridMapping->GetCellValue(i, 0).mb_str());
            printf("进入映射条目：%s\n", tmp);
            this->LoadFolderFiles2List(list, this->m_gridMapping->GetCellValue(i, 0), this->m_gridMapping->GetCellValue(i, 1));
        }
        else {
            this->LoadFile2List(list, this->m_gridMapping->GetCellValue(i, 0), this->m_gridMapping->GetCellValue(i, 1));
        }
    }

    //载入源（当前版本）文件表
    list = this->SrcFilesList;
    list->DesPath.Clear();
    list->SrcPath.Clear();
    list->md5.Clear();
    list->size.Clear();

    proj_info_t proj = wxGetApp().GetProjInfo();

    char SrcPath[1024];
    char ProjPath[1024];
    strcpy(ProjPath, proj.ProjPath.mb_str());
    sprintf(SrcPath, "%s/release/%ld.%ld.xml", ProjPath, proj.AbsVer, proj.SubAbsVer);

    TiXmlDocument doc(SrcPath);
    if (!doc.LoadFile()) {
        printf("错误：无法载入XML文件：%s\n", SrcPath);
        return false;
    }
    #ifdef DEBUG
    else {
        printf("载入XML文件： %s\n", SrcPath);
    }
    #endif

    i = 0;
    TiXmlHandle hDoc(&doc);
    TiXmlHandle hUpdate = hDoc.ChildElement("update", 0).ChildElement("files", 0);
    TiXmlHandle DomFile = hUpdate.ChildElement("file", 0);

    while (DomFile.ToElement()) {
        list->DesPath.Add(wxString(DomFile.ToElement()->GetText(), wxConvLibc));
        list->SrcPath.Add(wxString(DomFile.ToElement()->Attribute("src"), wxConvLibc));
        list->md5.Add(wxString(DomFile.ToElement()->Attribute("md5"), wxConvLibc));
        const char* tsize = DomFile.ToElement()->Attribute("size");
        if (tsize) {
            list->size.Add(atol(tsize));
        }
        else {
            list->size.Add(0);
        }

        DomFile = hUpdate.ChildElement("file", ++i);
    }

    SetStatus(wxString("更新成功", wxConvLibc));
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
        if (DesPath != wxT("")) DesPathIn = DesPath + wxT("/") + DesPathIn;
        LoadFile2List(list, paths[i], DesPathIn);
    }

    return true;
}
bool RMupdateManagerConfig::LoadFile2List(fileinfo_t*& list, wxString SrcPath, wxString DesPath)
{
    char path[1024];
    char md5[33];
    FILE* fp;
    long size;
    void* buffer;

    strcpy(path, SrcPath.mb_str());

    fp = fopen(path, "rb");
    if (!fp) {
        printf("error: can not open %s\n", path );
        SetStatus(wxString("无法打开文件：", wxConvLibc) + SrcPath);
        return false;
    }

    this->SetStatus(wxString("正在载入文件：", wxConvLibc) + SrcPath);
    this->Refresh(false);

    #ifdef DEBUG
    char tmp[1024];
    strcpy(tmp, SrcPath.mb_str());
    printf("计算哈希值：%s\t", tmp);
    strcpy(tmp, DesPath.mb_str());
    #endif

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(size);
    fread(buffer, size, 1, fp);
    fclose(fp);
    md5hash(buffer, size, md5);
    printf("%s\n", md5);

    list->size.Add(size);
    list->SrcPath.Add(SrcPath);
    list->DesPath.Add(DesPath);
    list->md5.Add(wxString(md5, wxConvLibc));

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
    long UnflagSrcNum = src->md5.Count();
    long UnflagDesNum = des->md5.Count();
    long ModifiedNum = 0;

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
            if (src->DesPath[k] == des->DesPath[i]) break;
        }
        if (k != src->DesPath.GetCount()) {
            //找到了相对路径相同的文件
            UnflagSrcNum--;
            UnflagDesNum--;
            char srcmd5[33];
            char desmd5[33];
            strcpy(srcmd5, src->md5[k].mb_str());
            strcpy(desmd5, des->md5[k].mb_str());
            printf("srcmd5=%s, desmd5=%s\n", srcmd5, desmd5);
            if (src->md5[k] != des->md5[k]) ModifiedNum++;
        }
    }

    return UnflagSrcNum + UnflagDesNum + ModifiedNum;
}

bool RMupdateManagerConfig::SaveFilesList()
{
    proj_info_t proj = wxGetApp().GetProjInfo();
    char tmp[1024];
    char path[1024];
    char proj_path[1024];

    strcpy(proj_path, proj.ProjPath.mb_str());
    strcpy(path, proj_path);
    sprintf(tmp, "/release/%ld.%ld.xml", proj.AbsVer, proj.SubAbsVer);
    strcat(path, tmp);

    printf("xml path: %s\n", path);

    TiXmlDocument* doc = new TiXmlDocument(path);
    TiXmlElement* root = new TiXmlElement("update");
    doc->LinkEndChild(root);

    TiXmlElement* version = new TiXmlElement("version");
    version->LinkEndChild(new TiXmlText(proj.version.mb_str()));
    root->LinkEndChild(version);

    TiXmlElement* AbsVer = new TiXmlElement("AbsVer");
    sprintf(tmp, "%ld", proj.AbsVer);
    AbsVer->LinkEndChild(new TiXmlText(tmp));
    root->LinkEndChild(AbsVer);

    TiXmlElement* SubAbsVer = new TiXmlElement("SubAbsVer");
    sprintf(tmp, "%ld", proj.SubAbsVer);
    SubAbsVer->LinkEndChild(new TiXmlText(tmp));
    root->LinkEndChild(SubAbsVer);

    TiXmlElement* UpdateTime = new TiXmlElement("UpdateTime");
    sprintf(tmp, "%ld", proj.UpdateTime);
    UpdateTime->LinkEndChild(new TiXmlText(tmp));
    root->LinkEndChild(UpdateTime);

    //创建文件列表节点
    unsigned int i;
    TiXmlElement* files = new TiXmlElement("files");
    for (i = 0; i < DesFilesList->DesPath.GetCount(); i++) {
        TiXmlElement* file = new TiXmlElement("file");
	#ifdef RMUPDATE_ENCRYPT_FILE
        // 如果是加密发布，按照工程规范应该计算服务器上的文件的MD5值
        char* md5;
        char filepath[1024];
        char filename[1024];
        void* buffer;
        long buffer_size;
        FILE* fp_r;

        strcpy(filename, DesFilesList->DesPath[i].mb_str());
        md5 = encrypt_file_path(filename);;
        sprintf(filepath, "%s/release/res/%s.dat", proj_path, md5);
        fp_r = fopen(filepath, "rb");
        fseek(fp_r, 0, SEEK_END);
        buffer_size = ftell(fp_r);
        fseek(fp_r, 0, SEEK_SET);
        buffer = malloc(buffer_size);
        fread(buffer, buffer_size, 1, fp_r);

        md5hash(buffer, buffer_size, md5);
        file->SetAttribute("md5", md5);

        free(buffer);
        free(md5);
        fclose(fp_r);
	#else
		file->SetAttribute("md5", DesFilesList->md5[i].mb_str());
	#endif
        file->SetAttribute("size", DesFilesList->size[i]);
        file->SetAttribute("src", DesFilesList->SrcPath[i].mb_str());
        file->LinkEndChild(new TiXmlText(DesFilesList->DesPath[i].mb_str()));
        files->LinkEndChild(file);
    }
    root->LinkEndChild(files);

    doc->SaveFile();

    #ifdef RMUPDATE_ENCRYPT_FILE
    //生成加密的更新文件列表
    FILE* fp;
    void* buffer;
    long buffer_size;

    //读取原文件
    fp = fopen(path, "rb");
    fseek(fp, 0, SEEK_END);
    buffer_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(buffer_size);
    fread(buffer, buffer_size, 1, fp);
    fclose(fp);

    //加密然后写入
    strcat(path, ".dat");
    fp = fopen(path, "wb");
    if (!fp) {
        printf("错误：无法以写模式打开文件 %s\n", path);
    }
    else {
        long tmplong;
        encrypt_file_content(buffer, buffer_size, tmplong);
        fwrite(buffer, buffer_size, 1, fp);
        fclose(fp);
    }
    #endif

    return true;
}

bool RMupdateManagerConfig::UpdateUpdateFile()
{
    proj_info_t proj = wxGetApp().GetProjInfo();
    FILE* fp;
    char buffer[2000];
    char path[1024];
    strcpy(path, proj.ProjPath.mb_str());
    strcat(path, "/release/update.xml");

    fp = fopen(path, "w");
    #if defined(__UNIX__)
    if (!fp) {
        char CDirPath[1024];

        strcpy(CDirPath, path);
        CDirPath[strrchr(path, '/') - path] = 0;
        printf("错误：创建文件失败，试图创建目录：%s\n", path);
        MKDIR(CDirPath);
        fp = fopen(path, "w");
    }
    #endif
    if (!fp) {
        wxMessageDialog(NULL, _T("无法写入更新文件"), _T("错误"));
        return false;
    }

    char version[1024];
    strcpy(version, proj.version.mb_str());
    sprintf(buffer, "<update><version>%s</version><AbsVer>%ld</AbsVer><SubAbsVer>%ld</SubAbsVer><UpdateTime>%ld</UpdateTime></update>", version, proj.AbsVer, proj.SubAbsVer, proj.UpdateTime);
    fwrite(buffer, strlen(buffer), 1, fp);
    fclose(fp);

    return true;
}

void RMupdateManagerConfig::OnDelete(wxCommandEvent& event)
{
    size_t i;
    proj_info_t proj = wxGetApp().GetProjInfo();
    wxArrayInt rows = m_gridMapping->GetSelectedRows();

    if (rows.GetCount() == 0) return;

    wxString SrcPath = m_gridMapping->GetCellValue(rows[0], 0);
    wxString DesPath = m_gridMapping->GetCellValue(rows[0], 1);

    if (m_gridMapping->GetCellValue(rows[0], 2) == wxT("目录")){
        for (i = 0; i < proj.MappingDirs.SrcPath.GetCount(); i++) {
            if (proj.MappingDirs.SrcPath[i] == SrcPath && proj.MappingDirs.DesPath[i] == DesPath) {
                proj.MappingDirs.SrcPath.RemoveAt(i);
                proj.MappingDirs.DesPath.RemoveAt(i);
            }
        }
    }
    else {
        for (i = 0; i < proj.MappingFiles.SrcPath.GetCount(); i++) {
            if (proj.MappingFiles.SrcPath[i] == SrcPath && proj.MappingFiles.DesPath[i] == DesPath) {
                proj.MappingFiles.SrcPath.RemoveAt(i);
                proj.MappingFiles.DesPath.RemoveAt(i);
            }
        }
    }
    wxGetApp().SetProjInfo(proj);

    m_gridMapping->DeleteRows(rows[0], 1, true);
}


bool RMupdateManagerConfig::UpdateResourceFiles()
{
    char DirPath[1024];
    char FilePath[1024];
    void* buffer;
    long buffer_size;
    FILE* fp;
    wxFile wxFp;
    proj_info_t proj = wxGetApp().GetProjInfo();
    fileinfo_t* list = DesFilesList;

    strcpy(DirPath, proj.ProjPath.mb_str());
    strcat(DirPath, "/release/res/");

    unsigned int i = 0;
    for (i = 0; i < list->SrcPath.Count(); i++) {
        //读取文件
        strcpy(FilePath, list->SrcPath[i].mb_str());
        fp = fopen(FilePath, "rb");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            buffer_size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            buffer = malloc(buffer_size);
            fread(buffer, buffer_size, 1, fp);
            fclose(fp);
        }

        //加密文件
    #ifdef RMUPDATE_ENCRYPT_FILE
        // 加密文件名
        char tmppath[1024];
        char* md5;
        strcpy(tmppath, list->DesPath[i].mb_str());
        strcpy(FilePath, DirPath);
        md5 = encrypt_file_path(tmppath);
        strcat(FilePath, md5);
        strcat(FilePath, ".dat");
        free(md5);

        // 加密文件内容。。！这里的内存没有处理好，按照加密函数中的定义，应该删除传入的buffer，而加密内容的指针是encrypt_file_content的返回值
        long tmplong;
        encrypt_file_content(buffer, buffer_size, tmplong);
    #else
        strcpy(FilePath, DirPath);
        strcat(FilePath, list->DesPath[i].mb_str());
    #endif

        //写入文件
        fp = fopen(FilePath, "wb");
        if (!fp) {
            char CDirPath[1024];

            strcpy(CDirPath, FilePath);
        #if defined(__WXMSW__)
            CDirPath[strrchr(FilePath, '\\') - FilePath] = 0;
        #elif defined(__UNIX__)
            CDirPath[strrchr(FilePath, '/') - FilePath] = 0;
        #endif
            printf("错误：创建文件失败，试图创建目录：%s\n", CDirPath);
            MKDIR(CDirPath)
            fp = fopen(FilePath, "wb");
        }

        if (fp) {
            fwrite(buffer, buffer_size, 1, fp);
            fclose(fp);
        }
        else {
            printf("错误：无法以写模式打开 %s\n", FilePath);
        }

        free(buffer);

    }

    return true;
}
