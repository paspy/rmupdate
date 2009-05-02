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
	if (!LoadConfig()) {
		wxMessageDialog(NULL, _T("can not load config file: update.conf"), _T("error"), wxOK | wxICON_ERROR).ShowModal();
		exit(1);
	}

    frame = new RMupdaterFrame(0L, _T("frame title"));

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
