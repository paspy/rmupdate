/***************************************************************
 * Name:      RMupdateManagerMain.h
 * Purpose:   Defines Application Frame
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-04-23
 * Copyright: gs (http://www.gsea.com.cn/)
 * License:
 **************************************************************/

#ifndef RMUPDATEMANAGERMAIN_H
#define RMUPDATEMANAGERMAIN_H



#include "RMupdateManagerApp.h"


#include "frameUI.h"

class RMupdateManagerFrame: public FrameProject
{
    public:
        RMupdateManagerFrame(wxFrame *frame);
        ~RMupdateManagerFrame();

		void RefreshProjInfo();
		void ProjectOpened(bool isOpened);
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnOnlineHelp(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void OnCreateProj( wxCommandEvent& event );
		virtual void OnOpenProj( wxCommandEvent& event );
		virtual void OnSaveProj( wxCommandEvent& event );
};

#endif // RMUPDATEMANAGERMAIN_H
