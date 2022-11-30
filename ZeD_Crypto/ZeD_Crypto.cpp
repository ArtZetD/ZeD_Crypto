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
		if (std::filesystem::is_directory(file))
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
		if (!EncryptFile(paths[i].string().c_str()))
		{
			if (GetLastError() == ERROR_FILE_READ_ONLY)
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
		if (!DecryptFile( paths[i].string().c_str(), NULL))
		{
			if (GetLastError() == ERROR_FILE_READ_ONLY)
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

int CALLBACK  wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)

{
	static std::vector<std::filesystem::path> paths;
	fill_paths(paths);

	MSG msg{};                             // Структура, которая содержит в себе информацию о соообщениях (между Windows и окном или между окнами).
	HWND hwnd{};                          // Дескриптор окна ( HANDLE указ. на объект ядра в котором храниться информация о нашем окне).
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) }; // Эта структура отвечает за некие х-ки окна (в фигурных скобках размеры).Исп. агрегатная инициализация.
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			HWND hBtnEncr = CreateWindow(
				TEXT("BUTTON"),
				TEXT("encrypt!"),
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				10, 100, 300, 50, hWnd, reinterpret_cast<HMENU>(1), nullptr, nullptr);

			HWND hBtnDecr = CreateWindow(
				TEXT("BUTTON"),
				TEXT("decrypt!"),
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				200, 300, 300, 50, hWnd, reinterpret_cast<HMENU>(2), nullptr, nullptr);
		}
		return 0;

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case 1:
				{
					if (encrypt(paths))
					{
						if (GetLastError() == ERROR_FILE_READ_ONLY)
						{
							MessageBox(hWnd, TEXT("ERROR_FILE_READ_ONLY"), TEXT("error"), MB_OK);
						}
						else
						{
							MessageBox(hWnd, TEXT("UNEXPECTED_ERROR"), TEXT("error"), MB_OK);
						}
						
					}
					
				}
				return 0;

				case 2:
				{
					if (decrypt(paths))
					{
						if (GetLastError() == ERROR_FILE_READ_ONLY)
						{
							MessageBox(hWnd, TEXT("ERROR_FILE_READ_ONLY"), TEXT("error"), MB_OK);
						}
						else
						{
							MessageBox(hWnd, TEXT("UNEXPECTED_ERROR"), TEXT("error"), MB_OK);
						}
					}
				}
				return 0;
			}
		}
		return 0;
		//case WM_:
		//{

		//}
		//return 0;

		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;


		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // вызывается в случае если сообщение не обрабатывается
	};
	wc.lpszClassName = TEXT("MyAppClass");
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;


	if (!RegisterClassEx(&wc)) // регистрация в системе класса нашего окна.
		return EXIT_FAILURE;

	if (hwnd = CreateWindow(wc.lpszClassName, TEXT("Заголовок!"), WS_OVERLAPPEDWINDOW, 0, 0, 600, 600, nullptr, nullptr, wc.hInstance, nullptr);
		hwnd == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;


	ShowWindow(hwnd, nCmdShow); // показ окна
	UpdateWindow(hwnd);         // перерисовка окна (передаётся HANDLE)

	while (GetMessage(&msg, nullptr, 0, 0)) // Цикл обработки сообщений
	{
		TranslateMessage(&msg); // функция расшифровывает системное сообщение
		DispatchMessage(&msg);  // функция  передаёт сообщение в оконную процедуру на обработку
	}

	return static_cast<int> (msg.wParam); // возвращаемое значение точки входа


}