/***************************************************************
 * Name:      RMupdaterApp.cpp
 * Purpose:   Code for Application Class
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

#include "RMupdaterApp.h"
#include "RMupdaterMain.h"
#include "ticpp/tinyxml.h"

IMPLEMENT_APP(RMupdaterApp);

bool RMupdaterApp::OnInit()
{
#ifdef RMUPDATE_ENCRYPT_FILE
	printf("RMupdater 加密版\n");
#else
	printf("RMupdater 普通版\n");
#endif

	// 载入程序图标
	wxInitAllImageHandlers();
	wxBitmap bitmap(_T("icon.png"), wxBITMAP_TYPE_PNG);
	icon.CopyFromBitmap(bitmap);

	// 载入配置文件
	if (!LoadConfig()) {
		wxMessageDialog((wxWindow*)this, _T("无法载入更新配置文件: update.conf"), _T("错误"), wxOK | wxICON_ERROR).ShowModal();
		exit(1);
	}

    frame = new RMupdaterFrame(0L, _T("RMupdater"));
    frame->Show();

    return true;
}

config_t RMupdaterApp::GetConfig()
{
	return config;
}

void RMupdaterApp::SetConfig(config_t c)
{
	config = c;
}

bool RMupdaterApp::LoadConfig()
{
	TiXmlDocument doc("update.conf");
	if (!doc.LoadFile()) return false;

	try {
		TiXmlHandle hDoc(&doc);
		TiXmlHandle root = hDoc.ChildElement("config", 0);

		const char* val;
		val = root.ChildElement("ServerUrl", 0).ToElement()->GetText();
		config.ServerPath = wxString(val, wxConvLibc);

		val = root.ChildElement("LastCheckTime", 0).ToElement()->GetText();
		config.LastCheckTime = val ? atol(val) : 0;

		val = root.ChildElement("AbsVer", 0).ToElement()->GetText();
		config.AbsVer = val ? atol(val) : 0;

		val = root.ChildElement("SubAbsVer", 0).ToElement()->GetText();
		config.SubAbsVer = val ? atol(val) : 0;

		val = root.ChildElement("CheckInterval", 0).ToElement()->GetText();
		config.CheckInterval = val ? atol(val) : 0;
	}
	catch(int) {
		return false;
	}

	return true;
}

void RMupdaterApp::SaveConfig()
{
	char tmp[1024];

	TiXmlDocument doc("update.conf");
	TiXmlElement* root = new TiXmlElement("config");
	doc.LinkEndChild(root);

	TiXmlElement* ServerUrl = new TiXmlElement("ServerUrl");
	ServerUrl->LinkEndChild(new TiXmlText(config.ServerPath.mb_str()));

	TiXmlElement* AbsVer = new TiXmlElement("AbsVer");
	sprintf(tmp, "%ld", config.AbsVer);
	AbsVer->LinkEndChild(new TiXmlText(tmp));

	TiXmlElement* SubAbsVer = new TiXmlElement("SubAbsVer");
	sprintf(tmp, "%ld", config.SubAbsVer);
	SubAbsVer->LinkEndChild(new TiXmlText(tmp));

	TiXmlElement* LastCheckTime = new TiXmlElement("LastCheckTime");
	sprintf(tmp, "%ld", config.LastCheckTime);
	LastCheckTime->LinkEndChild(new TiXmlText(tmp));

	TiXmlElement* CheckInterval = new TiXmlElement("CheckInterval");
	sprintf(tmp, "%ld", config.CheckInterval);
	CheckInterval->LinkEndChild(new TiXmlText(tmp));

	root->LinkEndChild(ServerUrl);
	root->LinkEndChild(AbsVer);
	root->LinkEndChild(SubAbsVer);
	root->LinkEndChild(LastCheckTime);
	root->LinkEndChild(CheckInterval);

	doc.SaveFile();
}

void RMupdaterApp::LoadUpdateFileList(TiXmlHandle* hDoc, file_list_t& list)
{
	long i;
	TiXmlHandle root = hDoc->ChildElement("update", 0);
	TiXmlHandle files = root.ChildElement("files", 0);

	// 先清除原有的数据
	list.DesPath.Clear();
	list.md5.Clear();
	list.size.Clear();

	// 从XML中载入数据
    i = 0;
    TiXmlHandle file = files.ChildElement("file", 0);
    while (file.ToElement()) {
    	list.DesPath.Add(wxString(file.ToElement()->GetText(), wxConvLibc));
    	list.md5.Add(wxString(file.ToElement()->Attribute("md5"), wxConvLibc));
    	const char* tsize = file.ToElement()->Attribute("size");
    	if (tsize != NULL) {
    		list.size.Add(atol(tsize));
    	}
    	else {
    		list.size.Add(0);
    	}
    	file = files.ChildElement("file", ++i);
    }
}

file_list_t RMupdaterApp::GetUpdateFileList()
{
	return UpdateList;
}

void RMupdaterApp::UpdateVersionInfo(version_t& ver)
{
	config_t c = GetConfig();
	c.AbsVer = ver.AbsVer;
	c.SubAbsVer = ver.SubAbsVer;
	SetConfig(c);
	printf("config set, absver=%ld, subabsver=%ld\n", ver.AbsVer, ver.SubAbsVer);
	SaveConfig();
	printf("更新文件保存了\n");
}

void RMupdaterApp::CompareUpdateList(file_list_t& ServerList, file_list_t& LocalList)
{
	unsigned long i, k;

	// 首先清空当前更新列表
	UpdateList.DesPath.Clear();
	UpdateList.md5.Clear();
	UpdateList.size.Clear();

	// 遍历服务器文件列表，并在本地文件列表中查找匹配项，如果匹配失败则增加到需要更新的文件列表结构去
	bool matched;
	for (i = 0; i < ServerList.DesPath.GetCount(); i++) {
		matched = false;
		for (k = 0; k < LocalList.DesPath.GetCount(); k++) {
			if (ServerList.DesPath[i] == LocalList.DesPath[k]) {
				if (ServerList.md5[i] == LocalList.md5[k]) matched = true;
				break;
			}
		}

		// 如果不匹配则需要更新
		if (!matched) {
			UpdateList.DesPath.Add(ServerList.DesPath[i]);
			UpdateList.md5.Add(ServerList.md5[i]);
			UpdateList.size.Add(ServerList.size[i]);
		}
	}
}
