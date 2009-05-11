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
	strcat(url, "update.xml");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RMupdaterFrame::curl_writefunction_check);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_in);
    curl_easy_perform(curl);

    if (curl_in.buffer_ptr != 0) {
    	((char*)curl_in.buffer)[curl_in.buffer_ptr] = 0;
    	((char*)curl_in.buffer)[curl_in.buffer_ptr+1] = 0;

    }

    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);

    if (http_code != 200 && http_code != 206) {
    	wxString info;
    	info.Printf(_T("下载更新文件时发生错误，HTTP错误代码：%ld"), http_code);
    	wxMessageDialog(NULL, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    	m_buttonCheck->Enable(true);
    	SetStatus(info);
    	return;
    }
    else {
    	TiXmlDocument doc;
		long absver, subabsver, updatetime;
		wxString version;

    	doc.Parse((const char*)curl_in.buffer);
    	if (doc.ErrorId() != 0) {
    		wxString info;
    		wxString errinfo;
    		errinfo = wxString(doc.ErrorDesc(), wxConvLibc);
    		info.Printf(_T("加载更新文件时发生错误：TiXmlError: ") + errinfo );
    		//printf("TiXmlError: line=%d, col%d, ErrId=%d\n", doc.ErrorRow(), doc.ErrorCol(), doc.ErrorId());
    		//printf("xml content:\n%s\n", (const char*)curl_in.buffer);
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
    		wxString info;
			struct tm *ttime;
			char timestr[100];
			wchar_t timestru[300];
			ttime = localtime(&updatetime);
			strftime(timestr, 100, "%Y年%m月%d日 %H时%M分%S秒", ttime);
			setlocale(LC_ALL,".ACP");
			mbstowcs(timestru, timestr, 300);

    		info.Printf(_T("发现新版本，发布时间是 %s"), timestru);
    		SetStatus(info);

			//下载更新列表文件
    		void* buf;
    		buf = DownloadUpdateList(absver, subabsver);
    		TiXmlDocument docl;
    		docl.Parse((const char*)buf);
    		if (docl.ErrorId() != 0) {
    			wxString TiErrInfo;
    			wxString ErrInfoL;
    			TiErrInfo = wxString(docl.ErrorDesc(), wxConvLibc);
    			ErrInfoL.Printf(_T("载入更新文件列表时发生错误：") + TiErrInfo);
    			wxMessageDialog(NULL, ErrInfoL, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    			return;
    		}
    		TiXmlHandle hDocL(&docl);
    		wxGetApp().LoadUpdateFileList(hDocL);

    		m_buttonUpdate->Enable(true);
    	}
    	else {
    		//没有更新
    		SetStatus(_T("没有更新"));
    	}
	}
}



void* RMupdaterFrame::DownloadUpdateList(long AbsVer, long SubAbsVer)
{
	writefunction_in_t curl_in;
	config_t config = wxGetApp().GetConfig();
	char url[1024];
	char ServerUrl[1024];
	strcpy(ServerUrl, config.ServerPath.mb_str());
	sprintf(url, "%s%ld.%ld.xml", ServerUrl, AbsVer, SubAbsVer);
	printf("%s\n", url);

	CURL* curl = curl_easy_init();
	curl_in.curl = curl;
	curl_in.buffer = NULL;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RMupdaterFrame::curl_writefunction_check);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_in);
	curl_easy_perform(curl);

	long http_code;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	curl_easy_cleanup(curl);

	if (http_code != 200 && http_code != 206) {
		wxString info;
		info.Printf(_T("下载更新列表文件时发生错误，HTTP错误代码：%ld"), http_code);
		wxMessageDialog(NULL, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
		SetStatus(info);
		return NULL;
	}

	return curl_in.buffer;
}

void RMupdaterFrame::ApplyUpdates()
{
}

size_t RMupdaterFrame::curl_writefunction_check(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t read_size = size * nmemb;setlocale(LC_ALL,".ACP");
    writefunction_in_t* in = (writefunction_in_t*)stream;

    double content_length;
    curl_easy_getinfo(in->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length);

    //检查HTTP代码
    double http_code;
    curl_easy_getinfo(in->curl, CURLINFO_RESPONSE_CODE, &http_code);
    //if (http_code != 200 && http_code != 206) return 0;

	//初始化缓冲区
    if (in->buffer == NULL) {
    	printf("init buffer for recive, malloc size=%lf\n", content_length + 1);
    	in->buffer = malloc(content_length + 1);
    	in->buffer_ptr = 0;
    }

    memcpy((char*)in->buffer + in->buffer_ptr, ptr, read_size);
    in->buffer_ptr += read_size;

    wxString info;
    info.Printf(_T("正在下载更新文件，共 %1$.0lf 字节，已经下载了 %2$ld 字节"), content_length, in->buffer_ptr);
    pFrameUpdater->SetStatus(info);
    pFrameUpdater->m_gaugeCurrent->SetValue(in->buffer_ptr * 100 / content_length);
    pFrameUpdater->m_gaugeTotal->SetValue(in->buffer_ptr * 100 / content_length);
    pFrameUpdater->m_gaugeCurrent->Update();
    pFrameUpdater->m_gaugeTotal->Update();

    return read_size;
}