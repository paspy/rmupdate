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

	return ret;
}

RMupdaterFrame::RMupdaterFrame(wxFrame *frame, const wxString& title)
    : FrameUpdater(frame, -1, title)
{
    m_buttonUpdate->Enable(false);
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
    m_buttonCheck->Enable(false);
    CheckNewest();
}

void RMupdaterFrame::OnUpdate(wxCommandEvent& event)
{
	if (DownloadUpdateFiles()) {
		// 更奔本地版本信息
		wxGetApp().UpdateVersionInfo(ServerVer);

		// 更改资源名，删除临时文件
		CleanUpUpdate();
		m_buttonStart->Enable(true);
	}
}

bool RMupdaterFrame::DownloadUpdateFiles()
{
	unsigned long i;
	file_list_t list = wxGetApp().GetUpdateFileList();

	m_buttonUpdate->Enable(false);

	//重制界面
	m_gaugeCurrent->SetValue(0);
	m_gaugeTotal->SetValue(0);

	MKDIR(".tmp");
	printf("mkdir\n");

	//依次下载
	for (i = 0; i < list.DesPath.GetCount(); i++) {
		FILE* fp;
		char path[2000];
		char* name_enc;

		//首先检查文件是否已经存在
		name_enc = encrypt_file_path(list.DesPath[i].mb_str());
		sprintf(path, ".tmp/%s.dat", name_enc);
		free(name_enc);

		fp = fopen(path, "r");
		printf("读取文件进行检查：%s\n", path);
		if (!fp) {
			if (!DownloadUpdateFile(list, i)) return false;
		}
		else {
			//如果文件存在，则检查文件是否是正确的文件
			printf("文件已经存在，进行检查\n");
			void* buffer;
			long buffer_size;
			char md5str[33];

			fseek(fp, 0, SEEK_END);
			buffer_size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			buffer = malloc(buffer_size);
			fread(buffer, buffer_size, 1, fp);
			md5hash(buffer, buffer_size, md5str);

			//如果哈希值不符，则从重新下载改文件
			char tmp[33];
			strcpy(tmp, list.md5[i].mb_str());
			printf("两个哈希值：%s\n%s\n", md5str, tmp);
			if (strcmp(md5str, list.md5[i].mb_str()) != 0) {
				if (!DownloadUpdateFile(list, i)) return false;
			}

			fclose(fp);
		}

		m_gaugeTotal->SetValue((i + 1) * 100 / list.DesPath.GetCount());
	}

	m_staticTextCurProc->SetLabel(_T("当前进度："));
	m_staticTextTotalProc->SetLabel(_T("总体进度"));
	m_statusBarInfo->SetStatusText(_T("更新文件下载完成"));

	// 调用应用更新的函数进行文件覆盖更新
	ApplyUpdates();

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
	strcpy(tmppath, list.DesPath[i].mb_str());
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
		return false;
	}

	wxString info;
	info = (_T("当前进度：正在下载 ") + list.DesPath[i]);
	m_staticTextCurProc->SetLabel(info);

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
	//curl_easy_cleanup(curl);

	free(filename);
	fclose(fp);

	long http_code;
	http_code = curl_in.http_code;
	if (http_code != 200 && http_code != 206 && http_code > 0 && http_code < 1024) {
		//似乎CURL会返回诡异的和ttp_code，值是一个很大的负数，一次调试的时候打印出来的十六进制是bf8dd0f8
    	wxString info;
    	info.Printf(_T("下载文件时发生错误，HTTP错误代码：%ld"), http_code);
    	printf("error downloading file: %x, %s\n", (unsigned int)http_code, url);
    	SetStatus(info);
    	return false;
    }

	return true;
}

void RMupdaterFrame::CheckNewest()
{
    writefunction_in_t curl_in;
    char url[1024];
	config_t config = wxGetApp().GetConfig();

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

    long http_code;
    http_code = curl_in.http_code;
    if (http_code != 200 && http_code != 206) {
    	wxString info;
    	info.Printf(_T("下载更新文件时发生错误，HTTP错误代码：%ld"), http_code);
    	wxMessageDialog(NULL, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    	m_buttonCheck->Enable(true);
    	SetStatus(info);
    	return;
    }
    else {
    	TiXmlDocument doc;
		wxString version;
		long updatetime;

    	doc.Parse((const char*)curl_in.buffer);
    	printf("xml=%s\n", (const char*)curl_in.buffer);
		free(curl_in.buffer);
    	if (doc.ErrorId() != 0) {
    		// 如果载入XML更新信息文件出错
    		wxString info;
    		wxString errinfo;
    		errinfo = wxString(doc.ErrorDesc(), wxConvLibc);
    		info.Printf(_T("加载更新文件时发生错误：TiXmlError: ") + errinfo );
    		wxMessageDialog(NULL, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    		m_buttonCheck->Enable(true);
    		SetStatus(info);
    		return;
    	}
    	else {
    		// 成功载入XML更新信息文件
    		TiXmlHandle hDoc(&doc);
    		TiXmlHandle root = hDoc.ChildElement("update", 0);

    		char tmp[1024];
    		const char* val;

			strcpy(tmp, root.ChildElement("version", 0).ToElement()->GetText());
			version = wxString(tmp, wxConvLibc);
			val = root.ChildElement("AbsVer", 0).ToElement()->GetText();
			ServerVer.AbsVer = val ? atol(val) : 0;
			val = root.ChildElement("SubAbsVer", 0).ToElement()->GetText();
			ServerVer.SubAbsVer = val ? atol(val) : 0;
			val = root.ChildElement("UpdateTime", 0).ToElement()->GetText();
			updatetime = val ? atol(val) : 0;
    	}

    	// 更新最后检查更新的时间
    	config.LastCheckTime = time(NULL);
    	wxGetApp().SetConfig(config);
    	wxGetApp().SaveConfig();
    	printf("最后更新时间已保存\n");

    	//检查更新情况
    	if (config.AbsVer != ServerVer.AbsVer || config.SubAbsVer != ServerVer.SubAbsVer) {
    		//有更新
    		wxString info;
			struct tm *ttime;
			char timestr[100];
			wchar_t timestru[300];
			ttime = localtime(&updatetime);
			strftime(timestr, 100, "%Y年%m月%d日 %H时%M分%S秒", ttime);
			setlocale(LC_ALL,".ACP");
			mbstowcs(timestru, timestr, 300);

    		info.Printf(_T("发现新版本，发布时间是 %s"), timestru);
    		SetStatus(info);

			//下载更新列表文件
    		void* buf;
    		buf = DownloadUpdateList(ServerVer.AbsVer, ServerVer.SubAbsVer);
    		TiXmlDocument docl;
    		docl.Parse((const char*)buf);
    		if (docl.ErrorId() != 0) {
    			wxString TiErrInfo;
    			wxString ErrInfoL;
    			TiErrInfo = wxString(docl.ErrorDesc(), wxConvLibc);
    			ErrInfoL.Printf(_T("载入更新文件列表时发生错误：") + TiErrInfo);
    			wxMessageDialog(NULL, ErrInfoL, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
    			return;
    		}
    		TiXmlHandle hDocL(&docl);
    		wxGetApp().LoadUpdateFileList(hDocL);

    		m_buttonUpdate->Enable(true);
    	}
    	else {
    		//没有更新
    		SetStatus(_T("没有更新"));
    	}
	}
}


void* RMupdaterFrame::DownloadUpdateList(long AbsVer, long SubAbsVer)
{
	writefunction_in_t curl_in;
	config_t config = wxGetApp().GetConfig();
	char url[1024];
	char ServerUrl[1024];
	strcpy(ServerUrl, config.ServerPath.mb_str());
	sprintf(url, "%s%ld.%ld.xml", ServerUrl, AbsVer, SubAbsVer);
	printf("%s\n", url);

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
		wxMessageDialog(NULL, info, _T("错误"), wxOK | wxICON_EXCLAMATION).ShowModal();
		SetStatus(info);
		return NULL;
	}

	return curl_in.buffer;
}

void RMupdaterFrame::ApplyUpdates()
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
	m_buttonStart->Enable(false);
	m_gaugeTotal->SetValue(0);
	m_gaugeCurrent->SetValue(0);
	m_staticTextCurProc->SetLabel(_T("当前进度：正在用更新文件覆盖旧文件"));
	m_staticTextCurProc->Update();

	rg_write = new rgss2a;
	rg_write->CreateRgss2aFile("Game.rgss2a.new");

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
		enc_filename = encrypt_file_path(list.DesPath[i].mb_str());
		sprintf(filename, ".tmp/%s.dat", enc_filename);

		// 设置读文件句柄
		fp = fopen(filename, "r");
		if (!fp) {
			printf("无法打开文件：%s\n", filename);
			wxMessageDialog(NULL, _T("无法打开文件：") + wxString(filename, wxConvLibc), _T("应用更新时发生错误"), wxICON_EXCLAMATION | wxID_OK).ShowModal();
			m_statusBarInfo->SetStatusText(_T("应用更新失败"));
			return;
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

		strcpy(filename_des, list.DesPath[i].mb_str());
		ApplyUpdateFile(filename_des, buffer, buffer_size);

		// 结束释放内存
		free (buffer);
		free (enc_filename);
		fclose(fp);
	}

	delete(rg_write);
}

bool RMupdaterFrame::ApplyUpdateFile(const char* despath, void* content, long content_size)
{
	char* despath_lower;
	printf("应用更新：%s\n", despath);

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
			wxMessageDialog(NULL, _T("无法以写模式打开文件：") + wxString(despath, wxConvLibc), _T("应用更新时发生错误"), wxICON_EXCLAMATION | wxID_OK).ShowModal();
			fclose(fp);
			return false;
		}

		printf("写入到：%s\n", despath);
		fwrite(content, 1, content_size, fp);

		fclose(fp);
	}

	free(despath_lower);
	return true;
}

void RMupdaterFrame::CleanUpUpdate()
{
	char packname[100];

	// 先处理打包的资源文件
	// --删除原来的资源文件
	if (remove("Game.rgss2a") == 0) {
		strcpy(packname, "Game.rgss2a");
	}
	else {
		remove("Game.rgssad");
		strcpy(packname, "Game.rgssad");
	}

	// --将新的资源文件改名
	printf("改名：Game.rgss2a.new --> %s\n", packname);
	rename("Game.rgss2a.new", packname);

	// 删除临时文件目录
#if defined(__WXMSW__)
	WinExec("cmd.exe /C rmdir .tmp /Q /S", SW_SHOW);
#elif defined(__UNIX__)
	char cmd[] = "rm -rf '.tmp'";
	printf("删除临时文件目录: %s\n", cmd);
	system(cmd);
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
    	printf("init buffer for recive, malloc size=%lf\n", content_length + 1);
    	in->buffer = malloc(content_length + 1);
    	in->buffer_ptr = 0;
    }

    memcpy((char*)in->buffer + in->buffer_ptr, ptr, read_size);
    in->buffer_ptr += read_size;

    wxString info;
    info.Printf(_T("正在下载更新文件，共 %1$.0lf 字节，已经下载了 %2$ld 字节"), content_length, in->buffer_ptr);
    pFrameUpdater->SetStatus(info);
    pFrameUpdater->m_gaugeCurrent->SetValue(in->buffer_ptr * 100 / content_length);
    pFrameUpdater->m_gaugeTotal->SetValue(in->buffer_ptr * 100 / content_length);
    pFrameUpdater->m_gaugeCurrent->Update();
    pFrameUpdater->m_gaugeTotal->Update();

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
	size_t size_down = ftell(in->fp);
	info.Printf(_T("共%1$.0lf字节，已下载 %2$ld 字节，速度 %3$.1lf KB/s"), content_length, size_down, speed_download / 1024);
	pFrameUpdater->SetStatus(info);
	pFrameUpdater->m_gaugeCurrent->SetValue(size_down * 100 / content_length);
	pFrameUpdater->m_gaugeCurrent->Update();
	wxGetApp().Yield();

	return read_size;
}
