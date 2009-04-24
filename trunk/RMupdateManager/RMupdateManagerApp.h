/***************************************************************
 * Name:      RMupdateManagerApp.h
 * Purpose:   Defines Application Class
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-04-23
 * Copyright: gs (http://www.gsea.com.cn/)
 * License:
 **************************************************************/

#ifndef RMUPDATEMANAGERAPP_H
#define RMUPDATEMANAGERAPP_H

#include <wx/app.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>

struct mapping_dirs_t{
    wxArrayString SrcPath;
    wxArrayString DesPath;
};

struct mapping_files_t{
    wxArrayString SrcPath;
    wxArrayString DesPath;
};

struct proj_info_t{
    wxString name;
    mapping_dirs_t MappingDirs;
    mapping_files_t MappingFiles;
    long AbsVer;
    long SubAbsVer;
    long UpdateTime;
};

class RMupdateManagerApp : public wxApp
{
    public:
        virtual bool OnInit();

        proj_info_t GetProjInfo();
        bool SaveProjInfo(proj_info_t proj);
        bool OpenProj();
        bool LoadProjConfig(const char* path);

    private:
       proj_info_t ProjInfo;

};

#endif // RMUPDATEMANAGERAPP_H
