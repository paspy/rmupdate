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
#include <wx/fileconf.h>
#include <wx/icon.h>

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
    bool modified;	//自上次保存工程或打开工程以来，工程是否被修改过了的标记
};


class RMupdateManagerApp : public wxApp
{
    public:

        virtual bool OnInit();
        RMupdateManagerApp();
        ~RMupdateManagerApp();

		void TryQuit();
        proj_info_t GetProjInfo();
        bool SetProjInfo(proj_info_t& proj);
        bool OpenProj();
        bool CreateProj();
        bool LoadProjConfig(const char* path);
        bool SaveProject();
        void ProjModified(bool isModified);
        bool GetProjModified();
        void ProjectOpened(bool isOpened);
        bool GetProjectOpened();
		long GetDateStamp();

		wxIcon icon;
		wxFileConfig* profile;

    private:
        proj_info_t ProjInfo;
        bool ProjOpened;

        bool CreateProjConfig(const char* path);
        void ReadUserProfile();
        bool SaveUserProfile();
        wxString GetUserProfilePath();
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


// define OPENLINK(URL)
//  this macro open a browser and follows the URL
#if defined(__WXMSW__)
	#define OPENLINK(URL)	\
		{ char cmd[4096];	\
		sprintf(cmd, "cmd.exe /C start %s", URL);	\
		WinExec(cmd, SW_HIDE); }
#elif defined(__UNIX__)
	#define OPENLINK(URL)	\
		{ char cmd[4096];	\
		sprintf(cmd, "firefox '%s'", URL);	\
		system(cmd); }
#endif
// end define OPENLINK(URL)


// define DPRINTF(__format, ...)
// 	this macro will print debug message to stdout acts like printf() when compile with DEBUG macro
#ifdef DEBUG
	#define DPRINTF(__format, ...)	\
		printf(__format, ## __VA_ARGS__);
#else
	#define DPRINTF(__format, ...)
#endif
