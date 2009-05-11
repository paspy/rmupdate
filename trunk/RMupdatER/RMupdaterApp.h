/***************************************************************
 * Name:      RMupdaterApp.h
 * Purpose:   Defines Application Class
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-05-01
 * Copyright: GPLv3
 * License:
 **************************************************************/

#ifndef RMUPDATERAPP_H
#define RMUPDATERAPP_H

#include <wx/app.h>
#include "RMupdaterUI.h"
#include "ticpp/tinyxml.h"

struct config_t{
	long AbsVer;
	long SubAbsVer;
	long LastCheckTime;
	long CheckInterval;
	wxString ServerPath;
};

struct file_list_t{
	wxArrayString DesPath;
	wxArrayString md5;
	wxArrayInt size;
};


class RMupdaterApp : public wxApp
{
	private:
		config_t config;
		bool LoadConfig();
		file_list_t UpdateList;

    public:
        virtual bool OnInit();

        FrameUpdater* frame;

        config_t GetConfig();
        void SetConfig(config_t c);
        void SaveConfig();
        void LoadUpdateFileList(TiXmlHandle& hDoc);
        file_list_t GetUpdateFileList();
};

#define pFrameUpdater ((RMupdaterFrame*)wxGetApp().frame)

#endif // RMUPDATERAPP_H
