#include<Windows.h>
#include <iostream>
#include<filesystem>
#include<tchar.h>
#include<vector>
#include <atlstr.h>
#include<fstream>
#include<iterator>
#include<cstdio>
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
	//DWORD d_logical_drive_string;
	GetLogicalDriveStrings(255, sz_buffer);
	//LPCTSTR removable_drive_ptr;
	std::vector<std::string> paths;

	for (buffer_ptr; *buffer_ptr != '\0'; buffer_ptr += _tcsclen(sz_buffer) + 1) 
	{

		if (GetDriveType(buffer_ptr) == DRIVE_REMOVABLE)
		{
			std::cout << "the removable drive has root name - ";
			get_files(paths, buffer_ptr);
			std::cout << '\n';

			for (size_t i = 2; i < paths.size(); ++i) // first 2 files is system file
			{
				std::string old_path = paths[i]; // 
				std::string new_path = paths[i]; // 
				const char point = '.';
				size_t name_start_iter = new_path.find_last_of('\\');
				size_t dot = new_path.find_last_of('.');
				//new_path.erase(name_start_iter, 7);


				const char* char_old_path = old_path.c_str();
				const char* char_new_path = new_path.c_str();

				std::cout << "before rename\n" << old_path << std::endl;

				new_path.insert(dot, "new");
				std::cout << "after rename\n" << new_path << std::endl;

				std::rename(char_old_path, char_new_path);

			}
		}
		else
		{
			continue;
		}
		
	}

	return 0;
}

