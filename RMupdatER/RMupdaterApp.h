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

struct config_t{
	long AbsVer;
	long SubAbsVer;
	long LastCheckTime;
	long CheckInterval;
	wxString ServerPath;
};

class RMupdaterApp : public wxApp
{
	private:
		config_t config;
		bool LoadConfig();

    public:
        virtual bool OnInit();

        FrameUpdater* frame;

        config_t GetConfig();
        void SetConfig(config_t c);
        void SaveConfig();
};

#define pFrameUpdater ((RMupdaterFrame*)wxGetApp().frame)

#endif // RMUPDATERAPP_H
