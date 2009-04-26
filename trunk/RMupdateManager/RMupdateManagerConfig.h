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

#include <wx/filedlg.h>

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
		virtual void OnCheckUpdate( wxCommandEvent& event );
		virtual void OnRelease( wxCommandEvent& event );
		virtual void OnTextChange( wxCommandEvent& event );
		virtual void OnDelete( wxCommandEvent& event );

		void SetStatus(wxString info);
		bool LoadFilesList();
		long CompareFilesList(fileinfo_t*& src, fileinfo_t*& des);    //返回不同的文件个数
		bool LoadFolderFiles2List(fileinfo_t*& list, wxString SrcPath, wxString DesPath);
		bool LoadFile2List(fileinfo_t*& list, wxString SrcPath, wxString DesPath);
		bool SaveFilesList();
		bool UpdateUpdateFile();

		bool MappingChanged;
		fileinfo_t* SrcFilesList;
		fileinfo_t* DesFilesList;
};

#endif // RMUPDATEMANAGERCONFIG_H
