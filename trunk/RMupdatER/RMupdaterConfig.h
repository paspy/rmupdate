/***************************************************************
 * Name:      RMupdaterConfig.h
 * Purpose:   Defines Application Frame
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-05-01
 * Copyright: GPLv3
 * License:
 **************************************************************/

#ifndef RMUPDATERCONFIG_H
#define RMUPDATERCONFIG_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "RMupdaterUI.h"
#include "RMupdaterApp.h"

class RMupdaterConfig: public DialogConfig
{
    public:
        RMupdaterConfig(wxFrame *frame, const wxString& title);
        ~RMupdaterConfig();
    private:
		void OnOK( wxCommandEvent& event );

};


#endif // RMUPDATERCONFIG_H
