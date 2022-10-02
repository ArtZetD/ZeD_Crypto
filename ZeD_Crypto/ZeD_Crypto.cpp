#include<Windows.h>
#include <iostream>
#include<filesystem>
#include<tchar.h>
#include<vector>
#include <atlstr.h>
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
	DWORD d_logical_drive_string;
	GetLogicalDriveStrings(255, sz_buffer);
	LPCTSTR removable_drive_ptr;
	std::vector<std::string> paths;

	for (buffer_ptr; *buffer_ptr != '\0'; buffer_ptr += _tcsclen(sz_buffer) + 1) 
	{

		if (GetDriveType(buffer_ptr) == DRIVE_REMOVABLE)
		{
			std::cout << "the removable drive has root name - ";
			get_files(paths, buffer_ptr);
			std::cout << '\n';
		}
	}

	for (size_t i = 0; i < paths.size(); ++i)
	{
		//std::cout << paths[i] << '\n';
		
	}
}

