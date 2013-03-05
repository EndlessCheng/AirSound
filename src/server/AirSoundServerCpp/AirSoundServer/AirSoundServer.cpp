// AirSoundServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AirSoundServer.h"
#include "Recorder.h"
#include "Server.h"
#include "VoiceRecording.h"
#include "VoicePlaying.h"
#include <ShellAPI.h>
#include <dwmapi.h>

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
HRESULT EnableBlurBehind(HWND hwnd);

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
	case WM_PAINT:
		{
			BOOL bCompEnabled;
			DwmIsCompositionEnabled(&bCompEnabled);
			if (bCompEnabled)
			{
				PAINTSTRUCT ps;
				HDC hDC = BeginPaint(g_hMainWnd, &ps);
				RECT rcClient;
				GetClientRect(g_hMainWnd, &rcClient);
				HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
				FillRect(hDC, &rcClient, hBrush);
				EndPaint(g_hMainWnd, &ps);
			}
		}
		break;
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
	EnableBlurBehind(g_hMainWnd);
}

void AddRecordDev(int id, string devName)
{
	HWND hComboWnd = GetDlgItem(g_hMainWnd, IDC_COMBO_DEVS);
	LRESULT index = SendMessage(hComboWnd, CB_ADDSTRING, NULL, (LPARAM)devName.c_str());
	SendMessage(hComboWnd, CB_SETITEMDATA, index, id);
}

HRESULT EnableBlurBehind(HWND hwnd)
{
    HRESULT hr = S_OK;

    // Create and populate the blur-behind structure.
    DWM_BLURBEHIND bb = {0};

    // Specify blur-behind and blur region.
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = true;
    bb.hRgnBlur = NULL;

    // Enable blur-behind.
    hr = DwmEnableBlurBehindWindow(hwnd, &bb);
	MARGINS margins = {-1};

    // Extend the frame across the whole window.
    hr = DwmExtendFrameIntoClientArea(hwnd,&margins);
    if (SUCCEEDED(hr))
    {
        // ...
    }
    return hr;
}

////���Ʒ�������
//void DrawGlowingText(HDC hDC, LPWSTR szText, RECT &rcArea, 
//	DWORD dwTextFlags = DT_LEFT | DT_VCENTER | DT_SINGLELINE, int iGlowSize = 10)
//{
//	//��ȡ������
//	HTHEME hThm = OpenThemeData(GetDesktopWindow(), L"TextStyle");
//	//����DIB
//	HDC hMemDC = CreateCompatibleDC(hDC);
//	BITMAPINFO bmpinfo = {0};
//	bmpinfo.bmiHeader.biSize = sizeof(bmpinfo.bmiHeader);
//	bmpinfo.bmiHeader.biBitCount = 32;
//	bmpinfo.bmiHeader.biCompression = BI_RGB;
//	bmpinfo.bmiHeader.biPlanes = 1;
//	bmpinfo.bmiHeader.biWidth = rcArea.right - rcArea.left;
//	bmpinfo.bmiHeader.biHeight = -(rcArea.bottom - rcArea.top);
//	HBITMAP hBmp = CreateDIBSection(hMemDC, &bmpinfo, DIB_RGB_COLORS, 0, NULL, 0);
//	if (hBmp == NULL) return;
//	HGDIOBJ hBmpOld = SelectObject(hMemDC, hBmp);
//	//����ѡ��
//	DTTOPTS dttopts = {0};
//	dttopts.dwSize = sizeof(DTTOPTS);
//	dttopts.dwFlags = DTT_GLOWSIZE | DTT_COMPOSITED;
//	dttopts.iGlowSize = iGlowSize;	//����ķ�Χ��С
//	//�����ı�
//	RECT rc = {0, 0, rcArea.right - rcArea.left, rcArea.bottom - rcArea.top};
//	HRESULT hr = DrawThemeTextEx(hThm, hMemDC, TEXT_LABEL, 0, szText, -1, dwTextFlags , &rc, &dttopts);
//	if(FAILED(hr)) return;
//	BitBlt(hDC, rcArea.left, rcArea.top, rcArea.right - rcArea.left, 
//		rcArea.bottom - rcArea.top, hMemDC, 0, 0, SRCCOPY | CAPTUREBLT);
//	//Clear
//	SelectObject(hMemDC, hBmpOld);
//	DeleteObject(hBmp);
//	DeleteDC(hMemDC);
//	CloseThemeData(hThm);
//}