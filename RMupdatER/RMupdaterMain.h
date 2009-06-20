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

#include "rgss2a.h"

struct writefunction_in_t{
    CURL* curl;
    void* buffer;
    long buffer_ptr;
    long http_code;
    FILE* fp;
    file_list_t* filelist;
};

class RMupdaterFrame: public FrameUpdater
{
    public:
        RMupdaterFrame(wxFrame *frame, const wxString& title);
        ~RMupdaterFrame();

        void AutomaticAction();
    private:
		rgss2a* rg_write;
		rgss2a* rg_read;

		version_t LocalVer;
		version_t ServerVer;

		file_list_t ServerList;
		file_list_t LocalList;

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnCheck(wxCommandEvent& event);
        void OnUpdate(wxCommandEvent& event);
        void OnConfig(wxCommandEvent& event);
        void OnStart(wxCommandEvent& event);

        void SetStatus(wxString info);
        bool CheckNewest();
        bool ApplyUpdates();
        void CleanUpUpdate();
        static size_t curl_writefunction_check(void *ptr, size_t size, size_t nmemb, void *stream);
        static size_t curl_writefunction_downfile(void *ptr, size_t size, size_t nmemb, void *stream);
        void* DownloadUpdateList(long AbsVer, long SubAbsVer, size_t& buf_size);
        void LoadUpdateList(TiXmlDocument* doc);
        bool DownloadUpdateFiles();
        bool DownloadUpdateFile(file_list_t& list, unsigned long i);
        bool ApplyUpdateFile(const char* despath, void* content, long content_size);
        void RefreshUpdateInfo(version_t& ver);
        void SetCurProcLabel(const wxString& info);
        void SetTtlProcLabel(const wxString& info);
        static wxString HumanReadSize(double speed_bytes);
        void ApplyNotUpdateRgss2a(file_list_t& ServerList, file_list_t& UpdateList);
        bool HashFile(const char* path, const char md5[33]);
};


#endif // RMUPDATERMAIN_H
