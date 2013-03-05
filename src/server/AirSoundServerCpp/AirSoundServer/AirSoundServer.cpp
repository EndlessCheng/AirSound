// AirSoundServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AirSoundServer.h"
#include "Recorder.h"
#include "Server.h"
#include "VoiceRecording.h"
#include "VoicePlaying.h"
#include <ShellAPI.h>

// ȫ�ֱ���:
HINSTANCE g_hInst;								// ��ǰʵ��
HICON g_hIcon;
HWND g_hMainWnd;
NOTIFYICONDATA g_nid = {};
// ���Ķ���
Recorder recorder;
Server server;
CVoiceRecording m_Record;						// ¼������
CVoicePlaying m_Play;

LRESULT CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void Init(HWND hDlgWnd);
void AddRecordDev(int id, string devName);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_hInst = hInstance;
	g_hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AIRSOUNDSERVER));

	INT_PTR result = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DialogProc);
	return result;
}

LRESULT CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		Init(hDlg);
		return TRUE;

	case WM_COMMAND:
		{
			int id = LOWORD(wParam);
			if (id == IDC_BUTTON_START)
			{
				server.Start();
			}
			if (id == ID_NOTITYICONMENU_EXIT)
			{
				Shell_NotifyIcon(NIM_DELETE,&g_nid);
				EndDialog(hDlg, 0);
			}
			break;
		}
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			ShowWindow(g_hMainWnd, SW_HIDE);  
		}
		break;

	case WM_AIRSOUND_NOTIFY:
		if(wParam == IDC_NOTIFYICON)
		{  
			if(lParam == WM_LBUTTONDOWN)
			{  
				ShowWindow(g_hMainWnd, SW_SHOWNORMAL);  
				SetForegroundWindow(g_hMainWnd);
				return TRUE;  
			}
			if (lParam == WM_RBUTTONDOWN)
			{
				HMENU hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU_NOTIFYICON));
				POINT point;
				GetCursorPos(&point);
				TrackPopupMenu(hMenu, 
					TPM_RIGHTBUTTON, 
					point.x,
					point.y,
					0,
					g_hMainWnd,
					NULL);
				PostMessage(g_hMainWnd, WM_NULL, NULL, NULL);
			}
		}  
	}
	return FALSE;
}

void Init(HWND hDlgWnd)
{
	g_hMainWnd = hDlgWnd;
	// ���ô���ͼ��
	SendMessage(hDlgWnd, WM_SETICON, ICON_BIG, (LPARAM)g_hIcon);
	// ������е�¼���豸��ComboBox
	recorder.EnumDevs((EnumDevsProc)AddRecordDev);
	HWND hComboWnd = GetDlgItem(g_hMainWnd, IDC_COMBO_DEVS);
	// �����1�����ϵ�¼���豸����ѡ�е�һ��¼���豸
	int count = SendMessage(hComboWnd, CB_GETCOUNT, NULL, NULL);
	if (count !=  0)
	{
		SendMessage(hComboWnd, CB_SETCURSEL, NULL, 0);
	}	// ��������ͼ��
	g_nid.cbSize = sizeof(g_nid);
	g_nid.uID = IDC_NOTIFYICON;
	g_nid.uCallbackMessage = WM_AIRSOUND_NOTIFY;
	g_nid.hIcon = g_hIcon;
	g_nid.hWnd = g_hMainWnd;		// ����ָ����������ͼ����Ϣ�ľ������������Ƶ�����ͼ���ϣ�ͼ�꼴��ʧ
	g_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	strcpy(g_nid.szTip, TEXT("AirSoundServer"));
	Shell_NotifyIcon(NIM_ADD, &g_nid);
}

void AddRecordDev(int id, string devName)
{
	HWND hComboWnd = GetDlgItem(g_hMainWnd, IDC_COMBO_DEVS);
	LRESULT index = SendMessage(hComboWnd, CB_ADDSTRING, NULL, (LPARAM)devName.c_str());
	SendMessage(hComboWnd, CB_SETITEMDATA, index, id);
}
