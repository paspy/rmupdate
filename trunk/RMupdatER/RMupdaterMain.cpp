/***************************************************************
 * Name:      RMupdaterMain.cpp
 * Purpose:   Code for Application Frame
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


#include "RMupdaterMain.h"
#include "RMupdaterConfig.h"

#include <curl/curl.h>
#include "ticpp/tinyxml.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

DECLARE_APP(RMupdaterApp);


RMupdaterFrame::RMupdaterFrame(wxFrame *frame, const wxString& title)
    : FrameUpdater(frame, -1, title)
{
    m_buttonUpdate->Enable(false);
}


RMupdaterFrame::~RMupdaterFrame()
{
}

void RMupdaterFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
    exit(0);
}

void RMupdaterFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
    exit(0);
}

void RMupdaterFrame::SetStatus(wxString info)
{
    m_statusBarInfo->SetStatusText(info);
}

void RMupdaterFrame::OnConfig(wxCommandEvent& event)
{
	RMupdaterConfig dlg(NULL, _T("设置"));
	dlg.ShowModal();
}

void RMupdaterFrame::OnCheck(wxCommandEvent& event)
{
    m_buttonCheck->Enable(false);
    CheckNewest();
}

void RMupdaterFrame::OnUpdate(wxCommandEvent& event)
{
}

void RMupdaterFrame::CheckNewest()
{
    writefunction_in_t curl_in;
    char url[1024];
	config_t config = wxGetApp().GetConfig();

    CURL* curl = curl_easy_init();
    curl_in.curl = curl;
    curl_in.buffer = NULL;

	strcpy(url, config.ServerPath.mb_str());

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RMupdaterFrame::curl_writefunction_check);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_in);
    curl_easy_perform(curl);

    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);

    if (http_code != 200 && http_code != 206) {
    	wxString info;
    	info.Printf(_T("下载更新文件发生错误，HTTP错误代码：%ld"), http_code);
    	wxMessageDialog(NULL, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    	m_buttonCheck->Enable(true);
    	SetStatus(info);
    	return;
    }
    else {
    	TiXmlDocument doc;
		long absver, subabsver, updatetime;
		wxString version;

    	if (!doc.Parse((const char*)curl_in.buffer)) {
    		wxString info;
    		wxString errinfo;
    		errinfo = wxString(doc.ErrorDesc(), wxConvLibc);
    		info.Printf(_T("加载更新文件时发生错误：") + errinfo );
    		wxMessageDialog(NULL, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    		m_buttonCheck->Enable(true);
    		SetStatus(info);
    		return;
    	}
    	else {
    		TiXmlHandle hDoc(&doc);
    		TiXmlHandle root = hDoc.ChildElement("update", 0);

    		char tmp[1024];
    		const char* val;

			strcpy(tmp, root.ChildElement("version", 0).ToElement()->GetText());
			version = wxString(tmp, wxConvLibc);
			val = root.ChildElement("AbsVer", 0).ToElement()->GetText();
			absver = val ? atol(val) : 0;
			val = root.ChildElement("SubAbsVer", 0).ToElement()->GetText();
			subabsver = val ? atol(val) : 0;
			val = root.ChildElement("UpdateTime", 0).ToElement()->GetText();
			updatetime = val ? atol(val) : 0;
    	}

    	//检查更新情况
    	if (config.AbsVer != absver || config.SubAbsVer != subabsver) {
    		//有更新
    		m_buttonUpdate->Enable(true);
    	}
    	else {
    		//没有更新
    	}

	}


}

void RMupdaterFrame::ApplyUpdates()
{
}

size_t RMupdaterFrame::curl_writefunction_check(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t read_size = size * nmemb;
    writefunction_in_t* in = (writefunction_in_t*)stream;

    double content_length;
    curl_easy_getinfo(in->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length);

    //检查HTTP代码
    double http_code;
    curl_easy_getinfo(in->curl, CURLINFO_RESPONSE_CODE, &http_code);
    //if (http_code != 200 && http_code != 206) return 0;

	//初始化缓冲区
    if (in->buffer == NULL) {
    	in->buffer = malloc(content_length + 1);
    	in->buffer_ptr = 0;
    }

    printf("size=%d, nmemb=%d\n", size, nmemb);
    memcpy((char*)in->buffer + in->buffer_ptr, ptr, read_size);
    in->buffer_ptr += read_size;

    wxString info;
    info.Printf(_T("正在下载更新文件，共 %1$ld 字节，已经下载了 %2$ld 字节"), content_length, in->buffer_ptr);
    pFrameUpdater->SetStatus(info);
    pFrameUpdater->m_gaugeCurrent->SetValue(in->buffer_ptr * 100 / content_length);
    pFrameUpdater->m_gaugeTotal->SetValue(in->buffer_ptr * 100 / content_length);
    pFrameUpdater->m_gaugeCurrent->Update();
    pFrameUpdater->m_gaugeTotal->Update();

    return read_size;
}
