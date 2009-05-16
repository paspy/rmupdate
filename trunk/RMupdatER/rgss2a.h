#ifndef RGSS2A_H_INCLUDED
#define RGSS2A_H_INCLUDED

#define RGSS2A_MAGIC_KEY	0xDEADCAFE

class rgss2a{
	private:
		FILE* fp_r;
		FILE* fp_w;
		long magic_key;

		long magic_key_E(long magic_key_orn);		// E变换
		void encrypt_content(void* buffer, unsigned long buffer_size, long content_magic_key);
		
	public:
		rgss2a();
		~rgss2a();
		bool OpenRgss2aFile(const char* path);
		bool CreateRgss2aFile(const char* path);
		bool ReadSubFile(char* &filename, void* &content, unsigned long& content_size);
		bool WriteSubFile(const char* filename, void* content, unsigned long content_size);
};
#endif // RGSS2A_H_INCLUDED
