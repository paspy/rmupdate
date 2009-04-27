#ifndef
#define FILE_ENCRYPT_INCLUDE

void* encrypt_file_content(void* file_content_buffer, long buffer_size);
void* decrypt_file_content(void* content_buffer, long buffer_size);

char* encrypt_file_path(char* filepath);
char* decrypt_file_path(char* filepath_chiper);

#endif
