#ifndef RMUPDATEMANAGERABOUT_H_INCLUDED
#define RMUPDATEMANAGERABOUT_H_INCLUDED

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "frameUI.h"
#include "RMupdateManagerApp.h"

class RMupdateManagerAbout: public DialogAbout
{
    public:
        RMupdateManagerAbout(wxFrame *frame, const wxString& title);
        ~RMupdateManagerAbout();
    private:
		void OnQuit( wxCommandEvent& event );
};

#endif // RMUPDATEMANAGERABOUT_H_INCLUDED
