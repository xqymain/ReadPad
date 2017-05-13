//����ͷ�ļ�
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#include "resource.h"

#pragma comment(lib,"Shlwapi.lib")

//�궨��
#define IDC_EDIT 1001
//ȫ�ֱ���
HINSTANCE g_hInstance;
TCHAR g_szFilePath[MAX_PATH];  //���ļ�·��
TCHAR g_szFileTitle[MAX_PATH];
//��������
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//��ں��� WinMain()
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;
	//���������� 
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0; //��֮����ڴ�ռ�Ĭ���ֽ���
	WndClass.cbWndExtra = 0; //����ʵ��Ĭ�϶�����ֽ���
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);     //������� ��ˢ
	WndClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));  //���ع��
	WndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(MB_ICONERROR));    //����ͼ��
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WindowProc; // ���ڻص�����
	WndClass.lpszClassName = TEXT("XinhReadPad");                 //������ TEXT�����ַ�ת��
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);            //�˵���Դ
	WndClass.style = CS_HREDRAW | CS_VREDRAW;                     //���
	//ע�ᴰ����
	RegisterClass(&WndClass);
	//��������
	HWND hWnd = CreateWindow(TEXT("XinhReadPad"),
		TEXT("���ʱ�"),
		WS_OVERLAPPEDWINDOW,
		500,
		30,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);
	//��ʾ����
	ShowWindow(hWnd, SW_SHOW);
	//���´���
	UpdateWindow(hWnd);
	//��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);  //���������Ϣת��Ϊ�ַ���Ϣ
		DispatchMessage(&msg);   //���õ�����Ϣ�ַ������ڳ���
	}
	return 0;
}
//�ص����� WindowProc
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditWnd;
	HFONT hFont;
	switch (uMsg)
	{
	case WM_CREATE:
	  hEditWnd = CreateWindow(TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN,
			0,0,0,0,hWnd, (HMENU)IDC_EDIT, g_hInstance,NULL);//�����༭�ؼ�
	  hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, GB2312_CHARSET,
		  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		  DEFAULT_PITCH | FF_SWISS, _T("����"));//��������
	  //��������
	  SendMessage(hEditWnd, WM_SETFONT, (WPARAM)hFont, 0);
	  //���ý���
	  SetFocus(hEditWnd);
		break;
	case WM_SIZE:
		MoveWindow(hEditWnd, 0, 0, LOWORD(lParam),HIWORD(lParam),
			FALSE);//�ƶ����ڴ�С
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
			ofn.lpstrFilter = TEXT("�ı��ĵ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0\0");
			ofn.lpstrFile = g_szFilePath;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST;
			if (!GetOpenFileName(&ofn))
			{
				return 0;
			}
			//MessageBox(NULL, g_szFilePath, TEXT("��ʾ"), MB_OK);  //�����õ�·��
			//��ȡ�ļ�
			//�õ��ļ����
			HANDLE hOpenFile = CreateFile(g_szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

			if (hOpenFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(hWnd, _T("��ѡ�ļ����ܴ�"),NULL,MB_OK);
				return 0;
			}
			//��ȡ�ļ���С
			DWORD dwFileSize;
			DWORD dwReadSize;
			dwFileSize = GetFileSize(hOpenFile, NULL);
			//�����ڴ�
			TCHAR *pBuffer = (TCHAR*)malloc(dwFileSize + 1);
			//��ȡ�ļ�
			ReadFile(hOpenFile, pBuffer, dwFileSize, &dwReadSize, NULL);
			//��ջ�����
			pBuffer[dwFileSize] = '\0';
			//���ı���ʾ���ؼ�
			SetWindowText(hEditWnd, pBuffer);
			//���ñ���
			_tcscpy_s(g_szFileTitle, sizeof(g_szFileTitle), g_szFilePath);
			PathStripPath(g_szFileTitle);
			SetWindowText(hWnd, g_szFilePath);
			//�ͷ��ڴ�
			free(pBuffer);
		}
			break;
		case IDM_SAVE:
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd); //�ݻٴ���
		break;
	case WM_DESTROY:
		PostQuitMessage(0);  //�˳�����
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //���ô��ڻص�����������ϵͳ����
	
}