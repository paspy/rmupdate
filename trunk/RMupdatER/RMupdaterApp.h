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
#include "ticpp/tinyxml.h"

struct config_t{
	long AbsVer;
	long SubAbsVer;
	long LastCheckTime;
	long CheckInterval;
	wxString ServerPath;
};

struct file_list_t{
	wxArrayString DesPath;
	wxArrayString md5;
	wxArrayInt size;
};


class RMupdaterApp : public wxApp
{
	private:
		config_t config;
		bool LoadConfig();
		file_list_t UpdateList;

    public:
        virtual bool OnInit();

        FrameUpdater* frame;

        config_t GetConfig();
        void SetConfig(config_t c);
        void SaveConfig();
        void LoadUpdateFileList(TiXmlHandle& hDoc);
        void UpdateVersionInfo(config_t ver);	//更新本地配置文件的版本号
        file_list_t GetUpdateFileList();
};

#define pFrameUpdater ((RMupdaterFrame*)wxGetApp().frame)


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


#endif // RMUPDATERAPP_H
