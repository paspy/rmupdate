/***************************************************************
 * Name:      RMupdateManagerApp.cpp
 * Purpose:   Code for Application Class
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
#include "ticpp/tinyxml.h"

IMPLEMENT_APP(RMupdateManagerApp);
DECLARE_APP(RMupdateManagerApp);

bool RMupdateManagerApp::OnInit()
{
    RMupdateManagerFrame* frameProject = new RMupdateManagerFrame(0L);
    frameProject->Show();

    return true;
}

bool RMupdateManagerApp::OpenProj()
{
//    wxFileDialog dlg(NULL, wxT("Choose a video file"), wxT(""), wxT(""), wxT("Video files (*.avi,*.mkv,*.ogg,*.mpg,*.mpeg,*.rm,*.rmvb)|*.*|All files|*.*"), wxOPEN | wxFILE_MUST_EXIST);
    wxDirDialog dlg(NULL);

    if (dlg.ShowModal() != wxID_OK) return false;

    char ConfigPath[2056];
    strcpy(ConfigPath, dlg.GetPath().mb_str());
	printf("%s\n", ConfigPath);
	strcat(ConfigPath, "/config.xml");
	if (!this->LoadProjConfig(ConfigPath)) {
        wxMessageDialog(NULL, _T("这似乎不是一个正确的工程目录，在这个目录下没有找到正确的工程配置文件，或配置文件已损坏"), _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
	    return false;
	}

    return true;
}

bool RMupdateManagerApp::LoadProjConfig(const char* path)
{
    TiXmlDocument dom(path);
    if (!dom.LoadFile()) return false;

    TiXmlHandle hDoc(&dom);
    TiXmlHandle root = hDoc.ChildElement("project", 0);

    TiXmlElement* eName = (root.ChildElement("name", 0).ToElement());
    TiXmlElement* eVersion = (root.ChildElement("version", 0).ToElement());
    TiXmlElement* eAbsVer = (root.ChildElement("AbsVer", 0).ToElement());
    TiXmlElement* eSubAbsVer = (root.ChildElement("SubAbsVer", 0).ToElement());
    TiXmlElement* eUpdateTime = (root.ChildElement("UpdateTime", 0).ToElement());

    ProjInfo.name = wxString(eName->GetText(), wxConvUTF8);
    ProjInfo.version = wxString(eVersion->GetText(), wxConvUTF8);

    //注意，GetText()在字符串为空是会返回0x0，而atol是不会检查该值的，如果直接调用atol将导致SIGSEGV
    const char* tmp;
    tmp = eAbsVer->GetText();
    if (!tmp) return false;
    this->ProjInfo.AbsVer = atol(tmp);

    tmp = eSubAbsVer->GetText();
    if (!tmp) return false;
    this->ProjInfo.SubAbsVer = atol(tmp);

    tmp = eUpdateTime->GetText();
    if (!tmp) return false;
    this->ProjInfo.UpdateTime = atol(tmp);

    #if defined(__WXMSW__)
    wxChar slash = wxChar('\\');
    #else
    wxChar slash = wxChar('/');
    #endif
    ProjInfo.ProjPath = wxString(path, wxConvUTF8).Before(slash);

    //载入映射目录和文件表，当然先删除现有的列表
    this->ProjInfo.MappingDirs.SrcPath.Clear();
    this->ProjInfo.MappingDirs.DesPath.Clear();
    this->ProjInfo.MappingFiles.SrcPath.Clear();
    this->ProjInfo.MappingFiles.DesPath.Clear();

    int i = 0;
    TiXmlHandle DomDirs = root.ChildElement("MappingDirs", 0);
    TiXmlHandle DomDir = DomDirs.ChildElement("dir", 0);
    while (DomDir.ToElement()) {
        ProjInfo.MappingDirs.SrcPath.Add(wxString(DomDir.ChildElement("SrcPath", 0).ToElement()->GetText(), wxConvUTF8));
        ProjInfo.MappingDirs.DesPath.Add(wxString(DomDir.ChildElement("DesPath", 0).ToElement()->GetText(), wxConvUTF8));
        DomDir = DomDirs.ChildElement("dir", ++i);
    }

    i = 0;
    TiXmlHandle DomFiles = root.ChildElement("MappingFiles", 0);
    TiXmlHandle DomFile = DomFiles.ChildElement("file", 0);
    while (DomFile.ToElement()) {
        ProjInfo.MappingFiles.SrcPath.Add(wxString(DomFile.ChildElement("SrcPath", 0).ToElement()->GetText(), wxConvUTF8));
        ProjInfo.MappingFiles.DesPath.Add(wxString(DomFile.ChildElement("DesPath", 0).ToElement()->GetText(), wxConvUTF8));
        DomFile = DomFiles.ChildElement("file", ++i);
    }

    return true;
}

proj_info_t RMupdateManagerApp::GetProjInfo()
{
    return ProjInfo;
}

bool RMupdateManagerApp::CreateProj()
{
    //选择文件夹
    wxDirDialog dlg(NULL, _T("Choose project directory"), _T(""), wxDD_DIR_MUST_EXIST);

    if (dlg.ShowModal() != wxID_OK) return false;

    char ConfigFile[2048];
    char ConfigDir[2048];
    strcpy(ConfigDir, dlg.GetPath().mb_str());

    //检查是否已经存在一个工程
    FILE* fp;
    strcpy(ConfigFile, ConfigDir);
    strcat(ConfigFile, "/config.xml");
    fp = fopen(ConfigFile, "r");
    if (fp && wxMessageDialog(NULL, _T("该目录下似乎已经创建了一个工程，是否覆盖？"), _T("似乎已经存在工程"), wxYES | wxNO | wxICON_QUESTION).ShowModal() != wxID_YES) return false;

    //for unix, create folder first
#if defined(__UNIX__)
    char cmd[1024];
    sprintf(cmd, "mkdir -p %s/release/res", ConfigDir);
    system(cmd);
#endif

    if (!this->CreateProjConfig(ConfigDir)) return false;

    RMupdateManagerConfig* frameConfig = new RMupdateManagerConfig(0L);
    frameConfig->Show();
    return true;

}

bool RMupdateManagerApp::CreateProjConfig(const char* path)
{
    char ConfigPath[2048];
    char Path00[2048];
    char timestr[100];

    strcpy(ConfigPath, path);
    strcpy(Path00, path);
    strcat(ConfigPath, "/config.xml");
    strcat(Path00, "/0.0.xml");

    sprintf(timestr, "%ld", time(NULL));

    //创建XML文件
    try {
        TiXmlDocument* doc = new TiXmlDocument(ConfigPath);

        TiXmlElement* root = new TiXmlElement("project");
        doc->LinkEndChild(root);

        TiXmlElement* name = new TiXmlElement("name");
        name->LinkEndChild(new TiXmlText("未命名"));
        root->LinkEndChild(name);

        TiXmlElement* version = new TiXmlElement("version");
        version->LinkEndChild(new TiXmlText("初始版本"));
        root->LinkEndChild(version);

        TiXmlElement* AbsVer;
        AbsVer = new TiXmlElement("AbsVer");
        AbsVer->LinkEndChild(new TiXmlText("0"));
        root->LinkEndChild(AbsVer);

        TiXmlElement* SubAbsVer;
        SubAbsVer = new TiXmlElement("SubAbsVer");
        SubAbsVer->LinkEndChild(new TiXmlText("0"));
        root->LinkEndChild(SubAbsVer);

        TiXmlElement* UpdateTime;
        UpdateTime = new TiXmlElement("UpdateTime");
        UpdateTime->LinkEndChild(new TiXmlText(timestr));
        root->LinkEndChild(UpdateTime);

        root->LinkEndChild(new TiXmlElement("MappingDirs"));
        root->LinkEndChild(new TiXmlElement("MappingFiles"));

        doc->SaveFile();
        delete doc;

        //创建0.0.xml文件
        doc = new TiXmlDocument(Path00);
        TiXmlElement* update = new TiXmlElement("update");
        doc->LinkEndChild(update);

        TiXmlElement* AbsVer1;
        AbsVer1 = new TiXmlElement("AbsVer");
        AbsVer1->LinkEndChild(new TiXmlText("0"));

        TiXmlElement* SubAbsVer1;
        SubAbsVer1 = new TiXmlElement("SubAbsVer");
        SubAbsVer1->LinkEndChild(new TiXmlText("0"));

        TiXmlElement* UpdateTime1;
        UpdateTime1 = new TiXmlElement("UpdateTime");
        UpdateTime1->LinkEndChild(new TiXmlText(timestr));

        update->LinkEndChild(AbsVer1);
        update->LinkEndChild(SubAbsVer1);
        update->LinkEndChild(UpdateTime1);
        update->LinkEndChild(new TiXmlElement("files"));

        doc->SaveFile();

    }
    catch (int e) {
        wxMessageDialog(NULL, _T("无法创建配置文件"), _T("错误"),wxOK | wxICON_EXCLAMATION).ShowModal();
        return false;
    }


    //保存工程属性
    proj_info_t proj;
    proj.name = wxString("未命名", wxConvUTF8);
    proj.ProjPath = wxString(path, wxConvUTF8);
    proj.version = wxString("默认版本", wxConvUTF8);
    proj.AbsVer = proj.SubAbsVer = 0;
    this->SetProjInfo(proj);

    return true;
}

bool RMupdateManagerApp::SetProjInfo(proj_info_t proj)
{
    ProjInfo = proj;
    return true;
}

bool RMupdateManagerApp::SaveProject()
{
    //设置版本号，以及更新时间戳
    long DateStamp = GetDateStamp();
    if (ProjInfo.AbsVer == DateStamp) {
        ProjInfo.SubAbsVer++;
    }
    else {
        ProjInfo.AbsVer = DateStamp;
        ProjInfo.SubAbsVer = 0;
    }
    ProjInfo.UpdateTime = time(NULL);

    //创建XML文档
    try {
        char ConfigPath[1024];
        char tmp[1024];
        strcpy(ConfigPath, ProjInfo.ProjPath.mb_str());
        strcat(ConfigPath, "/config.xml");

        TiXmlDocument* doc = new TiXmlDocument(ConfigPath);

        TiXmlElement* root = new TiXmlElement("project");
        doc->LinkEndChild(root);

        TiXmlElement* name = new TiXmlElement("name");
        name->LinkEndChild(new TiXmlText(ProjInfo.name.mb_str()));
        root->LinkEndChild(name);

        TiXmlElement* version = new TiXmlElement("version");
        version->LinkEndChild(new TiXmlText(ProjInfo.version.mb_str()));
        root->LinkEndChild(version);

        TiXmlElement* AbsVer = new TiXmlElement("AbsVer");
        sprintf(tmp, "%ld", ProjInfo.AbsVer);
        AbsVer->LinkEndChild(new TiXmlText(tmp));
        root->LinkEndChild(AbsVer);

        TiXmlElement* SubAbsVer = new TiXmlElement("SubAbsVer");
        sprintf(tmp, "%ld", ProjInfo.SubAbsVer);
        SubAbsVer->LinkEndChild(new TiXmlText(tmp));
        root->LinkEndChild(SubAbsVer);

        TiXmlElement* UpdateTime = new TiXmlElement("UpdateTime");
        sprintf(tmp, "%ld", ProjInfo.UpdateTime);
        UpdateTime->LinkEndChild(new TiXmlText(tmp));
        root->LinkEndChild(UpdateTime);

        TiXmlElement* MappingDirs = new TiXmlElement("MappingDirs");
        root->LinkEndChild(MappingDirs);
        TiXmlElement* MappingFiles = new TiXmlElement("MappingFiles");
        root->LinkEndChild(MappingFiles);

        //创建映射条目的节点
        unsigned long i;
        for (i = 0; i < ProjInfo.MappingDirs.SrcPath.GetCount(); i++) {
            TiXmlElement* dir = new TiXmlElement("dir");
            TiXmlElement* SrcPath = new TiXmlElement("SrcPath");
            TiXmlElement* DesPath = new TiXmlElement("DesPath");
            SrcPath->LinkEndChild(new TiXmlText(ProjInfo.MappingDirs.SrcPath[i].mb_str()));
            DesPath->LinkEndChild(new TiXmlText(ProjInfo.MappingDirs.DesPath[i].mb_str()));
            dir->LinkEndChild(SrcPath);
            dir->LinkEndChild(DesPath);
            MappingDirs->LinkEndChild(dir);
        }
        for (i = 0; i < ProjInfo.MappingFiles.SrcPath.GetCount(); i++) {
            TiXmlElement* file = new TiXmlElement("file");
            TiXmlElement* SrcPath = new TiXmlElement("SrcPath");
            TiXmlElement* DesPath = new TiXmlElement("DesPath");
            SrcPath->LinkEndChild(new TiXmlText(ProjInfo.MappingFiles.SrcPath[i].mb_str()));
            DesPath->LinkEndChild(new TiXmlText(ProjInfo.MappingFiles.DesPath[i].mb_str()));
            file->LinkEndChild(SrcPath);
            file->LinkEndChild(DesPath);
            MappingFiles->LinkEndChild(file);
        }

        doc->SaveFile();
    }
    catch (int e) {
        wxMessageDialog(NULL, _T("保存工程失败"), _T("错误")).ShowModal();
        return false;
    }

    return true;
}

long RMupdateManagerApp::GetDateStamp()
{
    long SplitStamp = time(NULL) - DATESTAMP_ZERO;
    return SplitStamp / 86400;
}
