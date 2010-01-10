/***************************************************************
 * Name:      RMupdaterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-05-01
 * Copyright: GPLv3
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__


#include "RMupdaterMain.h"
#include "RMupdaterConfig.h"

#include <curl/curl.h>
#include "ticpp/tinyxml.h"
#include "lib/md5.h"
#include "lib/file_encrypt.h"
#include "rgss2a.h"

#include "config.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

DECLARE_APP(RMupdaterApp);

// 真麻烦，还要自己实现一个大小写转换函数
char* strtolower(const char* str){
	register long len, i;
	char* ret;

	len = strlen(str);
	ret = (char*)malloc(len + 1);

	for (i = 0; i < len; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			ret[i] = str[i] + 32;
		}
		else {
			ret[i] = str[i];
		}
	}
    ret[i] = 0;

	return ret;
}

void converttolower(char* str){
	register unsigned long i;
	for (i = 0; i < strlen(str); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] += 32;
		}
	}
}

RMupdaterFrame::RMupdaterFrame(wxFrame *frame, const wxString& title)
    : FrameUpdater(frame, -1, title)
{
    wxBitmap bitmap;

	// 设置图标
	SetIcon(wxGetApp().icon);
	bitmap.CopyFromIcon(wxIcon(_T("aaaa")));
	m_bitmapLogo->SetBitmap(bitmap);

	// 设置本地版本信息
	config_t c;
	c =  wxGetApp().GetConfig();

	LocalVer.AbsVer = c.AbsVer;
	LocalVer.SubAbsVer = c.SubAbsVer;
	LocalVer.UpdateTime = 0;
}


RMupdaterFrame::~RMupdaterFrame()
{
}

void RMupdaterFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
    exit(0);
}

void RMupdaterFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
    exit(0);
}

void RMupdaterFrame::OnStart(wxCommandEvent& event)
{
#if defined(__WXMSW__)
	WCHAR exepath[2000];
	WCHAR openstr[20];
	mbstowcs(openstr, "open", 5);
	mbstowcs(exepath, "Game.exe", 9);
	ShellExecute(NULL, openstr, exepath, NULL, NULL, SW_SHOW);
#elif defined(__UNIX__)
	char cmd[2000];
	sprintf(cmd, "wine '%s' &", "./Game.exe");
	printf("执行命令：%s\n", cmd);
	system(cmd);
#endif
}

void RMupdaterFrame::SetStatus(wxString info)
{
    m_statusBarInfo->SetStatusText(info);
}

void RMupdaterFrame::OnConfig(wxCommandEvent& event)
{
	RMupdaterConfig dlg(NULL, _T("设置"));
	dlg.ShowModal();
}

void RMupdaterFrame::OnCheck(wxCommandEvent& event)
{
    CheckNewest();
}

void RMupdaterFrame::OnUpdate(wxCommandEvent& event)
{
    if (!DownloadUpdateFiles()) {
		return;
    }

	ApplyUpdates();
}

// 减少代码量的宏，用于在信息文本框上显示信息
#define WRITE_INFOITEM(NAME, VALUE)	\
	m_richTextInfo->BeginBold();	\
	m_richTextInfo->WriteText(_T(NAME));	\
	m_richTextInfo->EndBold();	\
	m_richTextInfo->WriteText(VALUE);
void RMupdaterFrame::RefreshUpdateInfo(version_t& ver)
{

	wxString info;
	struct tm *ttime;
	char timestr[100];

	info.Printf(_T("%ld.%ld\n"), ver.AbsVer, ver.SubAbsVer);
	WRITE_INFOITEM("当前版本号：", info);

	info.Printf(_T("%ld.%ld\n"), LocalVer.AbsVer, LocalVer.SubAbsVer);
	WRITE_INFOITEM("本地版本号：", info);

	ttime = localtime(&ver.UpdateTime);
	strftime(timestr, 100, "%Y年%m月%d日 %H时%M分%S秒", ttime);
	info = wxString(timestr, wxConvUTF8);
	WRITE_INFOITEM("最新版本发布时间：", info);

	wxColour color;
	m_richTextInfo->Newline();m_richTextInfo->Newline();
	m_richTextInfo->BeginBold();
	if (ver.AbsVer == LocalVer.AbsVer && ver.SubAbsVer == LocalVer.SubAbsVer) {
		color.Set(0, 128, 0);
		m_richTextInfo->BeginTextColour(color);
		m_richTextInfo->WriteText(_T("现在的版本已经是最新的了"));
	}
	else {
		color.Set(255, 0, 0);
		m_richTextInfo->BeginTextColour(color);
		m_richTextInfo->WriteText(_T("有新版本可以更新："));
		m_richTextInfo->EndTextColour();
		m_richTextInfo->EndBold();

		color.Set(0, 0, 255);
		m_richTextInfo->BeginTextColour(color);
		m_richTextInfo->WriteText(ServerVer.ReleaseName);
	}
}

bool RMupdaterFrame::DownloadUpdateFiles()
{
	unsigned long i;
	file_list_t list = wxGetApp().GetUpdateFileList();

	m_buttonUpdate->Enable(false);
	m_buttonStart->Enable(false);

	//重制界面
	m_gaugeCurrent->SetValue(0);
	m_gaugeTotal->SetValue(0);

	MKDIR(".tmp");
	printf("mkdir\n");

	//依次下载
	for (i = 0; i < list.DesPath.GetCount(); i++) {
		char path[2000];
		char* name_enc;

		//首先检查文件是否已经存在
		name_enc = encrypt_file_path(list.DesPath[i].mb_str(wxConvUTF8));
    #if defined(__WXMSW__)
        sprintf(path, ".tmp\\%s.dat", name_enc);
    #elif defined(__UNIX__)
		sprintf(path, ".tmp/%s.dat", name_enc);
    #endif
		free(name_enc);

		// 校验文件，并下载
		wxString info;

	#ifdef RMUPDATE_ENCRYPT_FILE
		info.Printf(_("正在检查第%1$lu个临时文件（共%2$lu个）"), i, list.DesPath.GetCount());
	#else
		info.Printf(_("正在检查临时文件：") + list.DesPath[i]);
	#endif
		m_statusBarInfo->SetStatusText(info);

		char md5[33];
		int try_times;

		try_times = 3;
		strcpy(md5, list.md5[i].mb_str());

		while (!HashFile(path, md5)) {
			DownloadUpdateFile(list, i);
			printf("--try_time=%d\n", try_times);
			if (--try_times < 0) {
				m_buttonUpdate->Enable(true);
				m_buttonStart->Enable(true);
				return false;
			}
		}

		// 设置界面
		m_gaugeTotal->SetValue((i + 1) * 100 / list.DesPath.GetCount());
	}

	SetCurProcLabel(_T(""));
	SetTtlProcLabel(wxEmptyString);
	m_statusBarInfo->SetStatusText(_T("更新文件下载完成"));

	return true;
}

bool RMupdaterFrame::DownloadUpdateFile(file_list_t& list, unsigned long i)
{
	writefunction_in_t curl_in;
	char url[1024];
	char filepath[1024];
	char* filename;
	CURL* curl;
	config_t config = wxGetApp().GetConfig();

	// 计算文件路径
	char tmppath[1024];
	strcpy(tmppath, list.DesPath[i].mb_str(wxConvUTF8));
	printf("--tmppath=%s\n", tmppath);
	filename = encrypt_file_path(tmppath);

	// 设置文件路径
	strcpy(filepath, ".tmp/");
	strcat(filepath, filename);
	strcat(filepath, ".dat");

	FILE* fp;
	fp = fopen(filepath, "wb");
	if (fp == NULL) {
		m_statusBarInfo->SetStatusText(_T("无法以写模式打开临时文件"));
		printf("can not open file to write: %s\n", filepath);
		free(filename);
		return false;
	}
	printf("--下载保存文件句柄路径:%s\n", filepath);

	wxString info;
	unsigned long FilesCount;
	FilesCount = list.DesPath.GetCount();
#ifdef RMUPDATE_ENCRYPT_FILE
	info.Printf(_("正在下载更新所需的文件..."), i, FilesCount);
#else
	info.Printf(_("正在下载：") + list.DesPath[i]);
#endif
	SetCurProcLabel(info);
	info.Printf(_("剩余%1$ld个文件，正在下载第%2$ld个，共%3$ld个"), FilesCount - i, i, FilesCount);
	SetTtlProcLabel(info);

	curl = curl_easy_init();
	curl_in.curl = curl;
	curl_in.buffer = NULL;
	curl_in.fp = fp;
	curl_in.filelist = &list;

	strcpy(url, config.ServerPath.mb_str());
	strcat(url, "res/");
	strcat(url, filename);
	strcat(url, ".dat");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RMupdaterFrame::curl_writefunction_downfile);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_in);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	free(filename);
	fclose(fp);

	long http_code;
	http_code = curl_in.http_code;
	if (http_code != 200 && http_code != 206 && http_code > 0 && http_code < 1024) {
		//似乎CURL会返回诡异的和ttp_code，值是一个很大的负数，一次调试的时候打印出来的十六进制是bf8dd0f8
    	wxString info;
    	info.Printf(_T("下载文件时发生错误，HTTP错误代码：%ld"), http_code);
    	printf("error downloading file: %ld, %s\n", http_code, url);
    	SetStatus(info);
    	return false;
    }

	return true;
}

bool RMupdaterFrame::CheckNewest()
{
    writefunction_in_t curl_in;
    char url[1024];
	config_t config = wxGetApp().GetConfig();

	// 设置界面
	m_buttonCheck->Enable(false);
	SetStatus(_("正在连接服务器..."));

    CURL* curl = curl_easy_init();
    curl_in.curl = curl;
    curl_in.buffer = NULL;

	strcpy(url, config.ServerPath.mb_str());
	strcat(url, "update.xml");
	printf("url=%s\n", url);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RMupdaterFrame::curl_writefunction_check);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_in);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

	pFrameUpdater->m_gaugeTotal->SetValue(33);
	pFrameUpdater->m_gaugeTotal->Update();

    long http_code;
    http_code = curl_in.http_code;
    if (http_code != 200 && http_code != 206) {
    	wxString info;
    	info.Printf(_T("下载更新文件时发生错误，HTTP错误代码：%ld"), http_code);
    	wxMessageDialog((wxWindow*)this, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    	m_buttonCheck->Enable(true);
    	SetStatus(info);

    	if (curl_in.buffer != NULL) free(curl_in.buffer);
    	return false;
    }
    else {
    	TiXmlDocument doc;
		wxString version;
		long updatetime;

    	doc.Parse((const char*)curl_in.buffer);
		free(curl_in.buffer);

    	if (doc.ErrorId() != 0) {
    		// 如果载入XML更新信息文件出错
    		wxString info;
    		wxString errinfo;
    		errinfo = wxString(doc.ErrorDesc(), wxConvLibc);
    		info.Printf(_T("加载更新文件时发生错误：TiXmlError: ") + errinfo );
    		wxMessageDialog((wxWindow*)this, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    		m_buttonCheck->Enable(true);
    		SetStatus(info);
    		return false;
    	}
    	else {
    		// 成功载入XML更新信息文件
    		TiXmlHandle hDoc(&doc);
    		TiXmlHandle root = hDoc.ChildElement("update", 0);

    		char tmp[1024];
    		const char* val;

			strcpy(tmp, root.ChildElement("version", 0).ToElement()->GetText());
			version = ServerVer.ReleaseName = wxString(tmp, wxConvUTF8);
			val = root.ChildElement("AbsVer", 0).ToElement()->GetText();
			ServerVer.AbsVer = val ? atol(val) : 0;
			val = root.ChildElement("SubAbsVer", 0).ToElement()->GetText();
			ServerVer.SubAbsVer = val ? atol(val) : 0;
			val = root.ChildElement("UpdateTime", 0).ToElement()->GetText();
			ServerVer.UpdateTime = updatetime = val ? atol(val) : 0;
    	}

    	// 更新最后检查更新的时间
    	config.LastCheckTime = time(NULL);
    	wxGetApp().SetConfig(config);
    	wxGetApp().SaveConfig();
    	printf("最后更新时间已保存\n");

    	//检查更新情况
    	if (config.AbsVer != ServerVer.AbsVer || config.SubAbsVer != ServerVer.SubAbsVer) {
    		//有更新
			//下载更新列表文件
    		void* buf_newest;
    		void* buf_current;
    		size_t size_newest, size_current;

			// 下载最新列表和当前列表，如果当前列表不存在则使用默认值
    		buf_newest = DownloadUpdateList(ServerVer.AbsVer, ServerVer.SubAbsVer, size_newest);
    		if (buf_newest == NULL) return false;
		#ifdef RMUPDATE_ENCRYPT_FILE
			long tmplong;
			decrypt_file_content(buf_newest, size_newest, tmplong);
		#endif
    		m_gaugeTotal->SetValue(66);
    		m_gaugeTotal->Update();

    		buf_current = DownloadUpdateList(LocalVer.AbsVer, LocalVer.SubAbsVer, size_current);
    		if (buf_current == NULL) buf_current = DownloadUpdateList(0, 0, size_current);
    		if (buf_current == NULL) {
    			m_statusBarInfo->SetStatusText(_T("无法下载当前版本的列表文件"));
    			free(buf_newest);
    			return false;
    		}
		#ifdef RMUPDATE_ENCRYPT_FILE
			decrypt_file_content(buf_current, size_current, tmplong);
		#endif
    		m_gaugeTotal->SetValue(100);
    		m_gaugeTotal->Update();

			// 载入文件列表
    		TiXmlDocument* docl;
    		TiXmlHandle* hDocL;
    		wxString TiErrInfo;
			wxString ErrInfoL;

			// --载入服务器最新列表
			docl = new TiXmlDocument;
    		docl->Parse((const char*)buf_newest);
    		if (docl->ErrorId() != 0) {
    			TiErrInfo = wxString(docl->ErrorDesc(), wxConvLibc);
    			ErrInfoL.Printf(_T("载入更新文件列表时发生错误：") + TiErrInfo);
    			wxMessageDialog((wxWindow*)this, ErrInfoL, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();

    			free(buf_newest);
    			free(buf_current);

    			return false;
    		}
    		hDocL = new TiXmlHandle(docl);
    		wxGetApp().LoadUpdateFileList(hDocL, ServerList);

    		delete hDocL;
    		delete docl;

			// --载入当前版本列表
			docl = new TiXmlDocument;
    		docl->Parse((const char*)buf_current);
    		if (docl->ErrorId() != 0) {
    			TiErrInfo = wxString(docl->ErrorDesc(), wxConvLibc);
    			ErrInfoL.Printf(_T("载入当前版本文件列表时发生错误：") + TiErrInfo);
    			wxMessageDialog((wxWindow*)this, ErrInfoL, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();

				free(buf_newest);
    			free(buf_current);

    			return false;
    		}
    		hDocL = new TiXmlHandle(docl);
    		wxGetApp().LoadUpdateFileList(hDocL, LocalList);

			delete hDocL;
			delete docl;

    		free(buf_current);
    		free(buf_newest);

			wxString info;
    		wxGetApp().CompareUpdateList(ServerList, LocalList);
    		info.Printf(_T("需要更新%ld个文件"), wxGetApp().GetUpdateFileList().DesPath.GetCount());
    		m_statusBarInfo->SetStatusText(info);
    		m_buttonUpdate->Enable(true);

    		RefreshUpdateInfo(ServerVer);
    		return true;
    	}
    	else {
    		//没有更新
    		SetStatus(_T("已经是最新版本了"));
    		m_gaugeTotal->SetValue(100);

    		RefreshUpdateInfo(ServerVer);
    		return false;
    	}
	}

}


void* RMupdaterFrame::DownloadUpdateList(long AbsVer, long SubAbsVer, size_t& buf_size)
{
	writefunction_in_t curl_in;
	config_t config = wxGetApp().GetConfig();
	char url[1024];
	char ServerUrl[1024];

	strcpy(ServerUrl, config.ServerPath.mb_str());
#ifdef RMUPDATE_ENCRYPT_FILE
	sprintf(url, "%s%ld.%ld.xml.dat", ServerUrl, AbsVer, SubAbsVer);
#else
	sprintf(url, "%s%ld.%ld.xml", ServerUrl, AbsVer, SubAbsVer);
#endif
	printf("下载更新列表文件：%s\n", url);

	CURL* curl = curl_easy_init();
	curl_in.curl = curl;
	curl_in.buffer = NULL;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RMupdaterFrame::curl_writefunction_check);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_in);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	long http_code;
	http_code = curl_in.http_code;
	if (http_code != 200 && http_code != 206) {
		wxString info;
		info.Printf(_T("下载更新列表文件时发生错误，HTTP错误代码：%ld"), http_code);
		if (!wxGetApp().ArgvSet.NoGui) wxMessageDialog((wxWindow*)this, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
		SetStatus(info);
		free(curl_in.buffer);
		return NULL;
	}

	buf_size = curl_in.buffer_ptr;
	return curl_in.buffer;
}

bool RMupdaterFrame::ApplyUpdates()
{
	file_list_t list = wxGetApp().GetUpdateFileList();
	unsigned long i, files_count;
	char* enc_filename;
	char filename[1024];
	char filename_des[1024];
	void* buffer;
	long buffer_size;
	FILE* fp;

	// 设置交互界面
	wxString tipinfo;
	tipinfo = _T("正在用更新文件覆盖旧文件");
	m_buttonStart->Enable(false);
	m_gaugeTotal->SetValue(0);
	m_gaugeCurrent->SetValue(0);
	SetCurProcLabel(tipinfo);
	m_statusBarInfo->SetStatusText(tipinfo);
	Update();

	rg_write = new rgss2a;
	rg_write->CreateRgss2aFile("Game.rgss2a.new");

	// 先将没有更新的资源包的文件重新写入一次
	rg_read = new rgss2a;
	if (rg_read->OpenRgss2aFile("Game.rgss2a") || rg_read->OpenRgss2aFile("Game.rgssad")) {
		m_statusBarInfo->SetStatusText(_("正在处理原资源包中的文件..."));
		Update();

		file_list_t UpList;
		UpList = wxGetApp().GetUpdateFileList();
		ApplyNotUpdateRgss2a(ServerList, UpList);
	}
	delete rg_read;

	m_statusBarInfo->SetStatusText(_("正在应用更新文件..."));
	Update();

	files_count = list.DesPath.GetCount();
	for (i = 0; i < files_count; i++) {
		long current_proc;

		// 显示当前进度
		current_proc = (i + 1) * 100 / files_count;
		m_gaugeTotal->SetValue(current_proc);
		m_gaugeCurrent->SetValue(current_proc);
		m_gaugeTotal->Update();
		m_gaugeCurrent->Update();

		// 加密文件名
		enc_filename = encrypt_file_path(list.DesPath[i].mb_str(wxConvUTF8));
		sprintf(filename, ".tmp/%s.dat", enc_filename);
		free (enc_filename);

		// 设置读文件句柄
		fp = fopen(filename, "rb");
		if (!fp) {
			// 操作失败了
			printf("无法打开文件：%s\n", filename);
			wxMessageDialog((wxWindow*)this, _T("无法打开文件：") + wxString(filename, wxConvLibc), _T("应用更新时发生错误"), wxICON_EXCLAMATION | wxID_OK).ShowModal();
			m_statusBarInfo->SetStatusText(_T("应用更新失败"));

			// 重置界面并返回假
			m_statusBarInfo->SetStatusText(_("更新失败"));
			m_buttonUpdate->Enable(true);
			m_buttonStart->Enable(true);

			return false;
		}
		else {
			fseek(fp, 0, SEEK_END);
			buffer_size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
		}

		// 应用更新
		buffer = malloc(buffer_size);
		fread(buffer, buffer_size, 1, fp);
	#ifdef RMUPDATE_ENCRYPT_FILE
		long tmplong;
		decrypt_file_content(buffer, buffer_size, tmplong);
	#endif

		strcpy(filename_des, list.DesPath[i].mb_str(wxConvUTF8));
		//  先替换反斜线
		unsigned long k;
		for (k = 0; k < strlen(filename_des); k++) {
		    if (filename_des[k] == '\\') filename_des[k] = '/';
		}
		ApplyUpdateFile(filename_des, buffer, buffer_size);

		// 结束释放内存
		free (buffer);
		fclose(fp);
	}

	SetCurProcLabel(_(""));
	delete(rg_write);

	// 更奔本地版本信息，更改资源名，删除临时文件
	wxGetApp().UpdateVersionInfo(ServerVer);
	CleanUpUpdate();

	// 重置界面
	m_buttonStart->Enable(true);
	m_buttonUpdate->Enable(false);
	m_buttonCheck->Enable(false);
	m_statusBarInfo->SetStatusText(_T("更新完成"));

	return true;
}

bool RMupdaterFrame::ApplyUpdateFile(const char* despath, void* content, long content_size)
{
	char* despath_lower;
	printf("应用更新：%s\n", despath);
#ifdef DEBUG
	printf("==[%s](%d) content_size=%ld\n", __func__, __LINE__, content_size);
#endif

	// 对于需要打包的文件和不需要打包的文件要分开处理
	despath_lower = strtolower(despath);
	if (
		strstr(despath_lower, "data/") == despath_lower ||
		strstr(despath_lower, "graphics/") == despath_lower
	){
		printf("--写入到rgss2a文件\n");
        rg_write->WriteSubFile(despath, content, content_size);
	}
	else {
		// 设置写文件句柄
		printf("--写入到普通文件\n");
		FILE* fp;
		fp = fopen(despath, "wb");
		if (fp == NULL) {
			char newdir[1024];
			strcpy(newdir, despath);
			newdir[strrchr(newdir, '/') - newdir] = 0;
			MKDIR(newdir);
			fp = fopen(despath, "wb");
		}

		if (fp == NULL) {
			printf("无法打开文件进行写入：%s\n", despath);
			wxMessageDialog((wxWindow*)this, _T("无法以写模式打开文件：") + wxString(despath, wxConvLibc), _T("应用更新时发生错误"), wxICON_EXCLAMATION | wxID_OK).ShowModal();
			fclose(fp);
			free(despath_lower);
			return false;
		}

		printf("----写入到：%s\n", despath);
		fwrite(content, 1, content_size, fp);

		fclose(fp);
	}

	free(despath_lower);
	return true;
}

void RMupdaterFrame::CleanUpUpdate()
{
#ifdef RESOURCE_PACK_NAME
    const char* packname = RESOURCE_PACK_NAME;
    remove(packname);
#else
	char packname[100] = {0};

	// 先处理打包的资源文件
	// --删除原来的资源文件
	if (remove("Game.rgss2a") == 0) {
		strcpy(packname, "Game.rgss2a");
	}
	else {
		remove("Game.rgssad");
		strcpy(packname, "Game.rgssad");
	}
#endif

	// --将新的资源文件改名
	printf("改名：Game.rgss2a.new --> %s\n", packname);
	rename("Game.rgss2a.new", packname);

	// 删除临时文件目录
#ifndef DEBUG
#if defined(__WXMSW__)
	WinExec("cmd.exe /C \"rd .tmp /Q /S\" ", SW_HIDE);
#elif defined(__UNIX__)
	char cmd[] = "rm -rf '.tmp'";
	printf("删除临时文件目录: %s", cmd);
	fflush(stdout);
	system(cmd);
	printf("\t删除完成\n");
#endif
#else
#error "DEBUG"
	printf("DEBUG版本不会删除临下载的临时文件\n");
#endif
}

size_t RMupdaterFrame::curl_writefunction_check(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t read_size = size * nmemb;
    writefunction_in_t* in = (writefunction_in_t*)stream;

    double content_length;
    curl_easy_getinfo(in->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length);
	curl_easy_getinfo(in->curl, CURLINFO_RESPONSE_CODE, &in->http_code);

	if (in->http_code != 200 && in->http_code != 206 && in->http_code != 0) {
		return 0;
	}

	//初始化缓冲区
    if (in->buffer == NULL) {
    	printf("[curl_writefunction_check] init buffer for recive, malloc size=%lf\n", content_length + 1);
    	in->buffer = malloc((int)content_length + 1);
    	in->buffer_ptr = 0;
    }

    memcpy((char*)in->buffer + in->buffer_ptr, ptr, read_size);
    in->buffer_ptr += read_size;

    wxString info;
#if defined (__WXMSW__)
	info.Printf(_("正在下载更新文件，共 %1$s，已经下载了 %2$s"),
#else
	info.Printf(_("正在下载更新文件，共 %1$S，已经下载了 %2$S"),
#endif
				(const wchar_t*)HumanReadSize(content_length).wc_str(),
				(const wchar_t*)HumanReadSize(in->buffer_ptr).wc_str()
	);
    pFrameUpdater->SetStatus(info);
    pFrameUpdater->m_gaugeCurrent->SetValue(in->buffer_ptr * 100 / (int)content_length);
    pFrameUpdater->m_gaugeCurrent->Update();

    return read_size;
}

size_t RMupdaterFrame::curl_writefunction_downfile(void *ptr, size_t size, size_t nmemb, void *stream){
	size_t read_size = size * nmemb;
	writefunction_in_t* in = (writefunction_in_t*)stream;

	double content_length, speed_download;

	curl_easy_getinfo(in->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length);
	curl_easy_getinfo(in->curl, CURLINFO_SPEED_DOWNLOAD, &speed_download);
	curl_easy_getinfo(in->curl, CURLINFO_RESPONSE_CODE, &in->http_code);

	if (in->http_code != 200 && in->http_code != 206 && in->http_code != 0) {
		return 0;
	}

	fwrite(ptr, size, nmemb, in->fp);

	wxString info;
	wxString a = _T("abc");
	size_t size_down = ftell(in->fp);
#if defined (__WXMSW__)
	info.Printf(_("共 %1$s，已下载 %2$s，速度 %3$s/秒"),
#else
	info.Printf(_("共 %1$S，已下载 %2$S，速度 %3$S/秒"),
#endif
				(const wchar_t*)HumanReadSize(content_length).wc_str(),
				(const wchar_t*)HumanReadSize(size_down).wc_str(),
				(const wchar_t*)HumanReadSize(speed_download).wc_str()
	);
	pFrameUpdater->SetStatus(info);
	pFrameUpdater->m_gaugeCurrent->SetValue(size_down * 100 / (int)content_length);
	pFrameUpdater->Update();
	wxGetApp().Yield();

	return read_size;
}

void RMupdaterFrame::SetCurProcLabel(const wxString& info)
{
	m_staticTextCurProc->SetLabel(_("当前进度：") + info);
}

void RMupdaterFrame::SetTtlProcLabel(const wxString& info)
{
	m_staticTextTotalProc->SetLabel(_("总体进度：") + info);
}

wxString RMupdaterFrame::HumanReadSize(double speed_bytes)
{
	wxString r;

	if (speed_bytes / 1024 < 1) {
		r.Printf(_("%.0lf 字节"), speed_bytes);
	}
	else if (speed_bytes / (1 << 20) < 1) {
		r.Printf(_("%.1lf 千字节"), speed_bytes / (1 << 10));
	}
	else if (speed_bytes / (1 << 30) < 1) {
		r.Printf(_("%.2lf 兆字节"), speed_bytes / (1 << 20));
	}

	return r;
}

void RMupdaterFrame::ApplyNotUpdateRgss2a(file_list_t& ServerList, file_list_t& UpdateList)
{
	void* content;
	unsigned long content_size, i, k;
	char* filename;
	bool isInServerList, isInUpdateList;
	char servernames[ServerList.DesPath.GetCount()][2048];
	char updatenames[UpdateList.DesPath.GetCount()][2048];

	if (!rg_read) wxASSERT(_("rg_read 未初始化"));

	// 将列表里面的文件读进普通字符串数组
	for (i = 0; i < ServerList.DesPath.GetCount(); i++) {
		strcpy(servernames[i], ServerList.DesPath[i].mb_str(wxConvUTF8));
		converttolower(servernames[i]);
		for (k = 0; k < strlen(servernames[i]); k++) {
			if (servernames[i][k] == '\\') servernames[i][k] = '/';
		}
	}
	for (i = 0; i < UpdateList.DesPath.GetCount(); i++) {
		strcpy(updatenames[i], UpdateList.DesPath[i].mb_str(wxConvUTF8));
		converttolower(updatenames[i]);
		for (k = 0; k < strlen(updatenames[i]); k++) {
			if (updatenames[i][k] == '\\') updatenames[i][k] = '/';
		}
	}

	// 依次读取旧资源包里面的文件进行检查
	while (rg_read->ReadSubFile(filename, content, content_size)) {
		wxString PackName;
		PackName = wxString(filename, wxConvUTF8);
		char packname[2048];
//printf("判断包里面的文件是否存在：%s\n", filename);
		strcpy(packname, PackName.mb_str(wxConvUTF8));
		converttolower(packname);

		for (k = 0; k < strlen(packname); k++) {
			if (packname[k] == '\\') packname[k] = '/';
		}

		// 检查该文件是否在更新列表中
		isInUpdateList = false;
		for (i = 0; i < UpdateList.DesPath.GetCount(); i++) {
			if (strcmp(updatenames[i], packname) == 0) {
				isInUpdateList = true;
				break;
			}
		}

		// 检查该文件是否在服务器列表中
		isInServerList = false;
		if (!isInUpdateList) {
			for (i = 0; i < ServerList.DesPath.GetCount(); i++) {
				//printf("比较字符串\n--源：%s\n--包：%s\n", servernames[i], packname);
				if (strcmp(servernames[i], packname) == 0) {
					isInServerList = true;
					break;
				}
			}
    }

		// 只需要写入即在服务器更新列表中，又不需要更新的文件
		if (isInServerList && !isInUpdateList) {
			// 把读出来的文件名中的反斜线换成斜线
			unsigned long k;
			for (k = 0; k < strlen(filename); k++) {
				if (filename[k] == '\\') filename[k] = '/';
			}
//printf("--写入包里面的文件：%s\n", filename);
			ApplyUpdateFile(filename, content, content_size);
		}

		// 释放分配的内存
		free(content);
		free(filename);
	}
}

bool RMupdaterFrame::HashFile(const char* path, const char md5[33])
{
	FILE* fp;
	file_list_t list = wxGetApp().GetUpdateFileList();

	fp = fopen(path, "rb");
	printf("读取文件进行检查：%s\n", path);

	if (fp == NULL) {
	    printf("--%s: 文件不存在，返回false\n", __func__);
		return false;
	}
	else {
		//如果文件存在，则检查文件是否是正确的文件
		printf("--%s: 文件已经存在，进行检查\n", __func__);
		void* buffer;
		long buffer_size;
		char md5str[33];

		fseek(fp, 0, SEEK_END);
		buffer_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		buffer = malloc(buffer_size);
		fread(buffer, buffer_size, 1, fp);
		md5hash(buffer, buffer_size, md5str);

		free(buffer);
		fclose(fp);

		//如果哈希值不符，则从重新下载改文件
		printf("--%s: 两个哈希值：\n----%s\n----%s\n", md5str, md5, __func__);
		if (strcmp(md5str, md5) != 0) return false;
	}

	return true;
}


void RMupdaterFrame::AutomaticAction()
{
	bool flag;
	argv_t a = wxGetApp().ArgvSet;

	flag = false;
	if (a.AutoCheck && (a.ForceCheck || wxGetApp().TimeToCheck())) {
		flag = CheckNewest();
	}

	Update();
	wxGetApp().Yield();
	if (a.AutoUpdate && flag && m_buttonUpdate->IsEnabled()) {
		flag = DownloadUpdateFiles();
	}

	if (a.AutoApply && a.AutoUpdate && flag) {
		flag = ApplyUpdates();
	}

	if (a.AutoStart) {
		wxCommandEvent e;
		OnStart(e);
	}

	if (a.AutoExit) {
		exit(0);
	}

}
