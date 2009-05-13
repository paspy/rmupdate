/***************************************************************
 * Name:      RMupdaterMain.h
 * Purpose:   Defines Application Frame
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-05-01
 * Copyright: GPLv3
 * License:
 **************************************************************/

#ifndef RMUPDATERMAIN_H
#define RMUPDATERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "RMupdaterUI.h"
#include "RMupdaterApp.h"

#include <curl/curl.h>

struct writefunction_in_t{
    CURL* curl;
    void* buffer;
    long buffer_ptr;
    FILE* fp;
    file_list_t* filelist;
};

class RMupdaterFrame: public FrameUpdater
{
    public:
        RMupdaterFrame(wxFrame *frame, const wxString& title);
        ~RMupdaterFrame();
    private:

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnCheck(wxCommandEvent& event);
        void OnUpdate(wxCommandEvent& event);
        void OnConfig(wxCommandEvent& event);

        void SetStatus(wxString info);
        void CheckNewest();
        void ApplyUpdates();
        static size_t curl_writefunction_check(void *ptr, size_t size, size_t nmemb, void *stream);
        static size_t curl_writefunction_downfile(void *ptr, size_t size, size_t nmemb, void *stream);
        void* DownloadUpdateList(long AbsVer, long SubAbsVer);
        void LoadUpdateList(TiXmlDocument* doc);
        bool DownloadUpdateFiles();
        bool DownloadUpdateFile(file_list_t& list, unsigned long i);
};


#endif // RMUPDATERMAIN_H
