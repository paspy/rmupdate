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

#define DATESTAMP_ZERO 978278400
//该时间戳是2001年1月1日0时0分0秒

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

struct fileinfo_t{
    wxArrayString SrcPath;  //这个是绝对路径
    wxArrayString DesPath;  //这个是相对于游戏目录的路径
    wxArrayString md5;
    WX_DEFINE_ARRAY_SIZE_T(unsigned long, size_a);
    size_a size;
};

struct proj_info_t{
    wxString name;
    wxString ProjPath;
    wxString version;
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
        bool SetProjInfo(proj_info_t proj);
        bool OpenProj();
        bool CreateProj();
        bool LoadProjConfig(const char* path);
        bool SaveProject();

    private:
        proj_info_t ProjInfo;

        bool CreateProjConfig(const char* path);
        long GetDateStamp();


};

#endif // RMUPDATEMANAGERAPP_H


// define MKDIR(EXP)
//  this cross-platform macro can make directory recuresively
//   the variable EXP should not include filename
#if defined(__WXMSW__)
    #define MKDIR(EXP)  \
        long __mkdir_stack[100];    \
        long __mkdir_stack_p = 0;   \
        char __mkdir_buf[2048];     \
		if (EXP[1] != ':') 		\
			sprintf(__mkdir_buf, ".\\%s", EXP);	\
		else 	\
			strcpy(__mkdir_buf, EXP);	\
        while (__mkdir_buf[++__mkdir_stack_p]) if (__mkdir_buf[__mkdir_stack_p] == '/') __mkdir_buf[__mkdir_stack_p] = '\\';  \
        __mkdir_stack_p = 0;    \
		while (-1 == _access(__mkdir_buf, 0)) { \
			__mkdir_stack[__mkdir_stack_p] = strrchr(__mkdir_buf, 92) - __mkdir_buf; \
			__mkdir_buf[__mkdir_stack[__mkdir_stack_p]] = 0;    \
			__mkdir_stack_p++;			\
		}   \
		while (--__mkdir_stack_p != -1) {   \
            __mkdir_buf[__mkdir_stack[__mkdir_stack_p]] = 92;   \
			_mkdir(__mkdir_buf);    \
		}   \
		_mkdir(__mkdir_buf);
#elif defined(__UNIX__)
    #define MKDIR(EXP)  \
        char __mkdir_cmd[2048]; \
        sprintf(__mkdir_cmd, "mkdir -p \"%s\"", EXP);   \
        system(__mkdir_cmd);
#endif
// end define MKDIR(EXP)

