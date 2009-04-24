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
#include "ticpp/tinyxml.h"

IMPLEMENT_APP(RMupdateManagerApp);
DECLARE_APP(RMupdateManagerApp);

bool RMupdateManagerApp::OnInit()
{
    RMupdateManagerFrame* frame = new RMupdateManagerFrame(0L);

    frame->Show();

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
    TiXmlElement* eAbsVer = (root.ChildElement("AbsVer", 0).ToElement());
    TiXmlElement* eSubAbsVer = (root.ChildElement("SubAbsVer", 0).ToElement());
    TiXmlElement* eUpdateTime = (root.ChildElement("UpdateTime", 0).ToElement());

    this->ProjInfo.name = wxString(eName->GetText(), wxConvUTF8);

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


    //载入映射目录和文件表
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
    return this->ProjInfo;
}
