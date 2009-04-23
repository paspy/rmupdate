/***************************************************************
 * Name:      RMupdateManagerApp.cpp
 * Purpose:   Code for Application Class
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-04-23
 * Copyright: gs (http://www.gsea.com.cn/)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "RMupdateManagerApp.h"
#include "RMupdateManagerMain.h"

IMPLEMENT_APP(RMupdateManagerApp);

bool RMupdateManagerApp::OnInit()
{
    RMupdateManagerFrame* frame = new RMupdateManagerFrame(0L);
    
    frame->Show();
    
    return true;
}
