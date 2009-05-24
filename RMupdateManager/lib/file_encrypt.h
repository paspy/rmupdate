#ifndef FILE_ENCRYPT_INCLUDE
#define FILE_ENCRYPT_INCLUDE

#ifdef RMUPDATE_ENCRYPT_FILE
//加密文件内容
// 加密后的内容指针 encrypt_file_content（欲加密的内容指针，欲加密的内容大小，加密后的内容大小）
void* encrypt_file_content(void* file_content_buffer, long buffer_size, long& buffer_size_rev);

//解密文件内容
// 解密后的内容指针 decrypt_file_content（欲解密的内容指针，欲解密的内容大小，解密后的内容大小）
void* decrypt_file_content(void* content_buffer, long buffer_size, long& buffer_size_rev);

//解密文件路径
char* decrypt_file_path(const char* filepath_chiper);

#endif // RMUPDATE_ENCRYPT_FILE

//加密文件路径
char* encrypt_file_path(const char* filepath);

#endif //FILE_ENCRYPT_INCLUDE
