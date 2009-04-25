/***************************************************************
 * Name:      RMupdateManagerConfig.h
 * Purpose:   Defines Application Frame
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-04-24
 * Copyright: gs (http://www.gsea.com.cn/)
 * License:
 **************************************************************/

#ifndef RMUPDATEMANAGERCONFIG_H
#define RMUPDATEMANAGERCONFIG_H



#include "RMupdateManagerApp.h"


#include "frameUI.h"

class RMupdateManagerConfig: public FrameConfig
{
    public:
        RMupdateManagerConfig(wxFrame *frame);
        ~RMupdateManagerConfig();
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
		virtual void OnAddFile( wxCommandEvent& event );
		virtual void OnAddDir( wxCommandEvent& event );
		virtual void OnCheck( wxCommandEvent& event );
		virtual void OnSave( wxCommandEvent& event );
		virtual void OnRelease( wxCommandEvent& event );

		void SetStatus(wxString& info);
};

#endif // RMUPDATEMANAGERCONFIG_H
