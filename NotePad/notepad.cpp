//包含头文件
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#include "resource.h"

#pragma comment(lib,"Shlwapi.lib")

//宏定义
#define IDC_EDIT 1001
//全局变量
HINSTANCE g_hInstance;
TCHAR g_szFilePath[MAX_PATH];  //打开文件路径
TCHAR g_szFileTitle[MAX_PATH];
//函数声明
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//入口函数 WinMain()
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;
	//构建窗口类 
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0; //类之后的内存空间默认字节数
	WndClass.cbWndExtra = 0; //窗口实例默认额外的字节数
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);     //软件背景 画刷
	WndClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));  //加载光标
	WndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(MB_ICONERROR));    //加载图标
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WindowProc; // 窗口回调函数
	WndClass.lpszClassName = TEXT("XinhReadPad");                 //类名称 TEXT代表字符转换
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);            //菜单资源
	WndClass.style = CS_HREDRAW | CS_VREDRAW;                     //风格
	//注册窗口类
	RegisterClass(&WndClass);
	//创建窗口
	HWND hWnd = CreateWindow(TEXT("XinhReadPad"),
		TEXT("访问本"),
		WS_OVERLAPPEDWINDOW,
		500,
		30,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);
	//显示窗口
	ShowWindow(hWnd, SW_SHOW);
	//更新窗口
	UpdateWindow(hWnd);
	//消息循环
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);  //将虚拟键消息转换为字符消息
		DispatchMessage(&msg);   //将得到的消息分发给窗口程序
	}
	return 0;
}
//回调函数 WindowProc
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditWnd;
	HFONT hFont;
	switch (uMsg)
	{
	case WM_CREATE:
	  hEditWnd = CreateWindow(TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN,
			0,0,0,0,hWnd, (HMENU)IDC_EDIT, g_hInstance,NULL);//创建编辑控件
	  hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, GB2312_CHARSET,
		  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		  DEFAULT_PITCH | FF_SWISS, _T("楷体"));//创建字体
	  //设置字体
	  SendMessage(hEditWnd, WM_SETFONT, (WPARAM)hFont, 0);
	  //设置焦点
	  SetFocus(hEditWnd);
		break;
	case WM_SIZE:
		MoveWindow(hEditWnd, 0, 0, LOWORD(lParam),HIWORD(lParam),
			FALSE);//移动窗口大小
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
			ofn.lpstrFilter = TEXT("文本文档(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0");
			ofn.lpstrFile = g_szFilePath;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST;
			if (!GetOpenFileName(&ofn))
			{
				return 0;
			}
			//MessageBox(NULL, g_szFilePath, TEXT("提示"), MB_OK);  //弹出得到路径
			//读取文件
			//得到文件句柄
			HANDLE hOpenFile = CreateFile(g_szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

			if (hOpenFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(hWnd, _T("所选文件不能打开"),NULL,MB_OK);
				return 0;
			}
			//获取文件大小
			DWORD dwFileSize;
			DWORD dwReadSize;
			dwFileSize = GetFileSize(hOpenFile, NULL);
			//开辟内存
			TCHAR *pBuffer = (TCHAR*)malloc(dwFileSize + 1);
			//读取文件
			ReadFile(hOpenFile, pBuffer, dwFileSize, &dwReadSize, NULL);
			//清空缓存区
			pBuffer[dwFileSize] = '\0';
			//将文本显示到控件
			SetWindowText(hEditWnd, pBuffer);
			//设置标题
			_tcscpy_s(g_szFileTitle, sizeof(g_szFileTitle), g_szFilePath);
			PathStripPath(g_szFileTitle);
			SetWindowText(hWnd, g_szFilePath);
			//释放内存
			free(pBuffer);
		}
			break;
		case IDM_SAVE:
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd); //摧毁窗口
		break;
	case WM_DESTROY:
		PostQuitMessage(0);  //退出进程
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //调用窗口回调函数，交于系统处理
	
}