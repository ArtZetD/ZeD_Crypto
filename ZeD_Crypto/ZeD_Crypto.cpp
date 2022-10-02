#include<Windows.h>
#include <iostream>
#include<filesystem>
#include<tchar.h>


int main()
{
	TCHAR szBuffer[256];
	LPCTSTR bufferPtr = szBuffer;
	DWORD d_logical_drive_string;
	GetLogicalDriveStrings(255, szBuffer);

	int counter = 0;


	for (bufferPtr; *bufferPtr != '\0'; bufferPtr += _tcsclen(szBuffer) + 1)
	{

		if (GetDriveType(bufferPtr) == DRIVE_REMOVABLE)
		{
			std::cout << "the removable drive has root name - ";
			std::wcout << bufferPtr << std::endl;
			static_cast<char>(*bufferPtr);
		}


	}
	std::cout << counter;
}

