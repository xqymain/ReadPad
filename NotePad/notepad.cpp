/*
 * Program Name: ReadPad
 * Author: xqymain
 * Under MIT protocol
 * Build Pass by Visual Stdio 2017
 */

// Include
//
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#include "resource.h"

#pragma comment(lib,"Shlwapi.lib")

// Defined
#define IDC_EDIT 1001

// Global variables
HINSTANCE g_hInstance;
TCHAR g_szFilePath[MAX_PATH];  // File Path
TCHAR g_szFileTitle[MAX_PATH];

// Function declaration
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Entrance function
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;
	// Create WndClass
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0; // The default number of bytes of memory space after class
	WndClass.cbWndExtra = 0; // The window instance defaults to extra bytes
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);     // White Background
	WndClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	WndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(MB_ICONERROR));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WindowProc; // WindowProc Function
	WndClass.lpszClassName = TEXT("ReadPad");                     // TEXT represents character conversion
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);            
	WndClass.style = CS_HREDRAW | CS_VREDRAW;                     
	
	RegisterClass(&WndClass);
	
	HWND hWnd = CreateWindow(TEXT("ReadPad"),
		TEXT("ReadPad"),
		WS_OVERLAPPEDWINDOW,
		500,
		30,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	ShowWindow(hWnd, SW_SHOW);
	
	UpdateWindow(hWnd);
	// Message Loop
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);  // Convert virtual key messages to character messages
		DispatchMessage(&msg);   // The message is distributed to the window procedure
	}
	return 0;
}
// Callback function WindowProc()
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditWnd;
	HFONT hFont;
	switch (uMsg)
	{
	case WM_CREATE:
	  hEditWnd = CreateWindow(TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN,
			0,0,0,0,hWnd, (HMENU)IDC_EDIT, g_hInstance,NULL);// Create an edit control
	  hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, GB2312_CHARSET,
		  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		  DEFAULT_PITCH | FF_SWISS, _T("楷体"));// Create Font
	  
	  SendMessage(hEditWnd, WM_SETFONT, (WPARAM)hFont, 0);
	  
	  SetFocus(hEditWnd);
		break;
	case WM_SIZE:
		MoveWindow(hEditWnd, 0, 0, LOWORD(lParam),HIWORD(lParam),
			FALSE);// Move Window size
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_NEW:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_OPEN:
		{
			OPENFILENAME ofn = { 0 };
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFilter = TEXT("Text File(*.txt)\0*.txt\0All File(*.*)\0*.*\0\0");
			ofn.lpstrFile = g_szFilePath;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST;
			if (!GetOpenFileName(&ofn))
			{
				return 0;
			}
			//MessageBox(NULL, g_szFilePath, TEXT("提示"), MB_OK);  //弹出得到路径
			// Read File
			// Get File Handle
			HANDLE hOpenFile = CreateFile(g_szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

			if (hOpenFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(hWnd, _T("The File can't open!"),NULL,MB_ICONERROR);
				return 0;
			}
			// Get File size
			DWORD dwFileSize;
			DWORD dwReadSize;
			dwFileSize = GetFileSize(hOpenFile, NULL);
			// Open up memory
			TCHAR *pBuffer = (TCHAR*)malloc(dwFileSize + 1);
			// Read file
			ReadFile(hOpenFile, pBuffer, dwFileSize, &dwReadSize, NULL);
			// Clear Buffer
			pBuffer[dwFileSize] = '\0';
			// Show Word to EditWnd
			SetWindowText(hEditWnd, pBuffer);
			// Set Title
			_tcscpy_s(g_szFileTitle, sizeof(g_szFileTitle), g_szFilePath);
			PathStripPath(g_szFileTitle);
			SetWindowText(hWnd, g_szFilePath);
			
			free(pBuffer);
		}
			break;
		case IDM_SAVE:
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
	
}
