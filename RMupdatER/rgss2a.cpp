/***************************************************************
 * Name:      rgss2a.cpp
 * Purpose:   Code for RGSS2A/RGSSAD File Handler
 * Author:    gs (gs@bbxy.net)
 * Created:   2009-05-16
 * Copyright: GPLv3
 * License:
 *
 * 注意：在一个实例中， OpenRgss2aFile 和 CreateRgss2aFile 方法是互斥的。
 *
 * 加密算法描述：http://blog.csdn.net/leexuany/archive/2008/08/24/2824795.aspx （leexuany(小宝)）
 **************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "rgss2a.h"
#include "string.h"

rgss2a::rgss2a()
{
	fp_r = fp_w = NULL;
}

rgss2a::~rgss2a()
{
	if (fp_r) fclose(fp_r);
	if (fp_w) fclose(fp_w);
}

long rgss2a::magic_key_E(long magic_key)
{
	return magic_key * 7 + 3;
}

bool rgss2a::OpenRgss2aFile(const char* path)
{
	if (fp_r != NULL) fclose(fp_r);
	fp_r = fopen(path, "rb");
	if (fp_r == NULL) return false;

	long tmp1, tmp2;
	fread(&tmp1, 4, 1, fp_r);
	fread(&tmp2, 4, 1, fp_r);
	if (0x53534752 != tmp1 || 0x01004441 != tmp2) {
		fclose(fp_r);
		fp_r = NULL;
	}

	magic_key = RGSS2A_MAGIC_KEY;

	return true;
}

bool rgss2a::CreateRgss2aFile(const char* path)
{
	if (fp_w != NULL) fclose(fp_w);
	fp_w = fopen(path, "wb");
	if (fp_w == NULL) return false;

	magic_key = RGSS2A_MAGIC_KEY;

	// 先写入文件头
	long tmp1, tmp2;
	tmp1 = 0x53534752;
	tmp2 = 0x01004441;
	fwrite(&tmp1, 4, 1, fp_w);
	fwrite(&tmp2, 4, 1, fp_w);

	return true;
}

bool rgss2a::ReadSubFile(char* &filename, void* &content, unsigned long& content_size)
{
	if (fp_r == NULL) return false;

	unsigned long path_size;
	unsigned long i;

	// 文件名长度
	fread(&path_size, 4, 1, fp_r);
	if (feof(fp_r)) return false;

	path_size ^= magic_key;
	magic_key = magic_key_E(magic_key);

	// 文件名
	filename = (char*)malloc(path_size + 1);
	fread(filename, path_size, 1, fp_r);
	for (i = 0; i < path_size; i++) {
		filename[i] ^= magic_key & 0xff;
		magic_key = magic_key_E(magic_key);
	}
	filename[path_size] = 0;

	// 文件内容长度
	fread(&content_size, 4, 1, fp_r);
	content_size ^= magic_key;
	magic_key = magic_key_E(magic_key);

	// 文件内容
	content = malloc(content_size + 4);
	fread(content, content_size, 1, fp_r);
	encrypt_content(content, content_size, magic_key);

	return true;
}

bool rgss2a::WriteSubFile(const char* filename, void* content, unsigned long content_size)
{
	unsigned long name_length, i;
	char* name;

	// 文件名长度
	name_length = strlen(filename);
	name_length ^= magic_key;
	fwrite(&name_length, 4, 1, fp_w);
	name_length ^= magic_key;
	magic_key = magic_key_E(magic_key);

	// 文件名
	name = (char*)malloc(name_length+100);
	memcpy(name, filename, name_length);
	for (i = 0; i < name_length; i++) {
		if (name[i] == '/') name[i] = '\\';		// Windwos下的目录是以反斜杠分隔的，斜杠的RM会无法识别
		name[i] ^= magic_key & 0xff;
		magic_key = magic_key_E(magic_key);
	}
	fwrite(name, name_length, 1, fp_w);
	free(name);

	// 文件长度
	content_size ^= magic_key;
	fwrite(&content_size, 4, 1, fp_w);
	content_size ^= magic_key;
	magic_key = magic_key_E(magic_key);

	// 文件内容
	encrypt_content(content, content_size, magic_key);
	fwrite(content, content_size, 1, fp_w);

	fflush(fp_w);

	return true;
}


void rgss2a::encrypt_content(void* buffer, unsigned long buffer_size, long content_magic_key)
{
	long* buf;
	unsigned long i;

	buf = (long*) buffer;

	for (i = 0; i <= (buffer_size -1) / 4; i++) {
		buf[i] ^= content_magic_key;
		content_magic_key = magic_key_E(content_magic_key);
	}
}
