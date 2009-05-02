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
    SetStatus(_T("checking updates..."));
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
    curl_easy_cleanup(curl);

    //


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

    if (in->buffer == NULL) {
    	in->buffer = malloc(content_length + 1);
    	in->buffer_ptr = 0;
    }

    printf("size=%d, nmemb=%d\n", size, nmemb);
    memcpy((char*)in->buffer + in->buffer_ptr, ptr, read_size);
    in->buffer_ptr += read_size;

    char info[1024];
    sprintf(info, "downloading update info... %ld bytes in %0.0lf bytes", in->buffer_ptr, content_length);
    pFrameUpdater->SetStatus(wxString(info, wxConvLibc));
    pFrameUpdater->m_gaugeCurrent->SetValue(in->buffer_ptr * 100 / content_length);
    pFrameUpdater->m_gaugeTotal->SetValue(in->buffer_ptr * 100 / content_length);
    pFrameUpdater->m_gaugeCurrent->Update();
    pFrameUpdater->m_gaugeTotal->Update();

    return read_size;
}
