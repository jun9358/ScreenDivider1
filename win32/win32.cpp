// win32.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"

#define MAX_LOADSTRING 100

// 전역 변수:
extern	TCHAR	g_formPath[MAX_PATH];

TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
HINSTANCE	g_hInst;
HWND		g_hListView;
HWND		g_hSettings[3];
BOOL		g_isHook;
TCHAR		g_LoadedFilePath[MAX_PATH] = TEXT("");
ATOM		g_atm;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32));

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_DLGFRAME,
      CW_USEDEFAULT, 0, SCREENDIVIDER_WIDTH, SCREENDIVIDER_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message)
	{
	case WM_HOTKEY:
		switch(wParam)
		{
		case HOTKEY_PAUSE:
			if (g_isHook)
			{
				g_isHook = FALSE;
				UnhookProc();
				LoadDataForDll("");
				ReLoadDataForDll();
			}
			else
			{
				g_isHook = TRUE;
				LoadDataForDll(g_LoadedFilePath);
				ReLoadDataForDll();
			}

			break;
		default:
			TCHAR	strFileName[MAX_PATH];
			TCHAR	strIndex[3];

			GetModuleFileName(NULL, strFileName, MAX_PATH);
			PathRemoveFileSpec(strFileName);
			lstrcat(strFileName, TEXT("\\Hotkey.ini"));

			// Get path
			_itoa_s(wParam, strIndex, 3, 5);
			GetPrivateProfileString(strIndex, TEXT("path"), 0, g_LoadedFilePath, MAX_PATH, strFileName);
			LoadDataForDll(g_LoadedFilePath);
			AutoRunProgram((LPSTR)g_LoadedFilePath);
		}

		break;
	case WM_CREATE:
		BOOL	ret;

		//Move to center
		MoveWindow(hWnd, (GetSystemMetrics(SM_CXSCREEN) - SCREENDIVIDER_WIDTH) / 2, (GetSystemMetrics(SM_CYSCREEN) - SCREENDIVIDER_HEIGHT) / 2,
			SCREENDIVIDER_WIDTH, SCREENDIVIDER_HEIGHT, TRUE);

		//Create list view control.
		g_hListView = CreateListView(g_hInst, hWnd);

		//Create setting dialogs
		g_hSettings[SETTING_NORMAL] = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_SETTING_NORMAL), hWnd, 0);
		g_hSettings[SETTING_HOTKEY] = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_SETTING_HOTKEY), hWnd, dlgSettingHotkeyProc);
		g_hSettings[SETTING_ABOUT] = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, 0);
		MoveWindow(g_hSettings[SETTING_NORMAL], 154, 12, 369, 247, TRUE);
		MoveWindow(g_hSettings[SETTING_HOTKEY], 154, 12, 369, 247, TRUE);
		MoveWindow(g_hSettings[SETTING_ABOUT], 154, 12, 369, 247, TRUE);
		ShowWindow(g_hSettings[SETTING_NORMAL], SW_SHOW);

		//Load settings
		LoadSettings();

		//Register hokeys
		RegisterHotkeys(hWnd);

		//Create buttons
		CreateWindow(TEXT("button"), TEXT("확인"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			SCREENDIVIDER_WIDTH - 80 - 73 - 73 - 48, 247 + 24, 73, 21,hWnd, (HMENU)IDC_BUTTON_OK, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("취소"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			SCREENDIVIDER_WIDTH - 80 - 73 - 36, 247 + 24, 73, 21, hWnd, (HMENU)IDC_BUTTON_CANCEL, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("적용"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			SCREENDIVIDER_WIDTH - 80 - 24, 247 + 24, 73, 21, hWnd, (HMENU)IDC_BUTTON_APPLY, g_hInst, NULL);

		//Create tray icon.
		CreateTray(hWnd);

		//Hook
		g_isHook = TRUE;
		ret = HookProc();
		if (!ret)
		{
			MessageBox(hWnd, "sdHook.dll 파일을 찾을 수 없습니다.", "Error", MB_ICONERROR);
			SendMessage(hWnd, WM_DESTROY, 0, 0);
		}

		break;
	case WM_NOTIFY:
		UINT	i;

		if (((NMHDR FAR*)lParam)->hwndFrom == g_hListView)
			switch(((LPNMHDR)lParam)->code)
			{
			case LVN_ITEMCHANGED:
				for (i=0 ; i<NUMITEM ; i++)
				{
					if (i == ((LPNMLISTVIEW)lParam)->iItem)
					{
						ShowWindow(g_hSettings[i], SW_SHOW);
						continue;
					}
					ShowWindow(g_hSettings[i], SW_HIDE);
				}

				break;
			}

		break;
	case WM_USER + 100:
		HMENU	hMenu, hPopup;
		POINT	pt;

		switch(LOWORD(lParam))
		{
		case WM_PAINT:

			break;

		case WM_RBUTTONDOWN:
			hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_TRAY));
			hPopup = GetSubMenu(hMenu, 0);

			GetCursorPos(&pt);

			TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
			DestroyMenu(hMenu);

			break;
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_TRAY_FORM_NEW:
			lstrcpy(g_formPath, "");
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_BOARD), hWnd, dlgBoardProc);

			ReLoadDataForDll();

			break;
		case IDM_TRAY_FORM_OPEN:
			OPENFILENAME	ofn;
			//TCHAR			lpstrFile[MAX_PATH];

			lstrcpy(g_LoadedFilePath, "");

			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = TEXT("Form Files(*.sdfrm)\0*.sdfrm\0");
			ofn.lpstrFile = g_LoadedFilePath;
			ofn.nMaxFile = MAX_PATH;
			if (GetOpenFileName(&ofn) == 0)
				break;
			LoadDataForDll((LPSTR)g_LoadedFilePath);
			AutoRunProgram((LPSTR)g_LoadedFilePath);

			break;
		case IDM_TRAY_FORM_EDIT:
			lstrcpy(g_formPath, g_LoadedFilePath);
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_BOARD), hWnd, dlgBoardProc);
			ReLoadDataForDll();

			break;
		case IDM_TRAY_SETTING:
			// Unregister hotkeys
			UnregisterHotkeys(hWnd);

			//Load settings
			LoadSettings();

			// Show
			ShowWindow(hWnd, SW_SHOW);

			break;
		case IDM_TRAY_EXIT:
			DestroyWindow(hWnd);

			break;

		case IDC_BUTTON_OK:
			SendMessage(hWnd, WM_COMMAND, MAKEWORD(IDC_BUTTON_APPLY, 0), 0);
			SendMessage(hWnd, WM_CLOSE, 0, 0);

			//RegisterHotkeys(hWnd);

			break;

		case IDC_BUTTON_CANCEL:
			SendMessage(hWnd, WM_CLOSE, 0, 0);

			RegisterHotkeys(hWnd);

			break;
		case IDC_BUTTON_APPLY:
			SaveSettings();

			RegisterHotkeys(hWnd);

			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		break;
	case WM_DESTROY:
		//DestroyTray icon.
		DestroyTray(hWnd);

		//Unhook
		UnhookProc();

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/*
// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
*/

BOOL CreateTray(HWND hWnd){
	NOTIFYICONDATA	nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WIN32));
	nid.hWnd = hWnd;
	lstrcpy(nid.szTip, szTitle);
	nid.uCallbackMessage = UM_TRAY;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uID = 100;

	return Shell_NotifyIcon(NIM_ADD, &nid);
}

BOOL DestroyTray(HWND hWnd){
	NOTIFYICONDATA	nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WIN32));
	nid.hWnd = hWnd;
	nid.uID = 100;

	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

BOOL AutoRunProgram(LPSTR lpFileName){
	UINT	i;
	UINT	numZone;
	TCHAR	stri[3];
	TCHAR	strtmp[MAX_STRING];

	numZone = GetPrivateProfileInt(TEXT("numZone"), TEXT("numZone"), 0, lpFileName);

	for (i=0 ; i<=numZone ; i++)
	{
		_itoa_s(i, (char *)stri, 3, 5);

		if (GetPrivateProfileInt(stri, TEXT("auto run"), 0, lpFileName))
		{
			// Path
			GetPrivateProfileString(stri, TEXT("path"), 0, strtmp, MAX_STRING, lpFileName);

			// Run
			ShellExecute(0, NULL, strtmp, NULL, NULL, TRUE);
		}
	}

	return TRUE;
}

BOOL LoadSettings(void){
	UINT	i;
	UINT	numHotkey;
	TCHAR	strFileName[MAX_PATH];
	TCHAR	stri[3];
	TCHAR	strHotkey[MAX_STRING];
	TCHAR	strPath[MAX_PATH];
	LPSTR	strItems[2];
	HWND	hHotkeyListView;

	// Get directory
	GetModuleFileName(NULL, strFileName, MAX_PATH);
	PathRemoveFileSpec(strFileName);
	lstrcat(strFileName, TEXT("\\Setting.ini"));

	if (PathFileExists(strFileName))
	{
		// Run when windows start
		SendDlgItemMessage(g_hSettings[SETTING_NORMAL], IDC_CHECK_RUN, BM_SETCHECK, GetPrivateProfileInt(TEXT("Run"), TEXT("value"), 0, strFileName), 0);
	}

	// Get directory
	PathRemoveFileSpec(strFileName);
	lstrcat(strFileName, TEXT("\\Hotkey.ini"));

	strItems[0] = strHotkey;
	strItems[1] = strPath;

	if (PathFileExists(strFileName))
	{
		numHotkey = GetPrivateProfileInt(TEXT("Hotkey"), TEXT("num"), 0, strFileName);
		hHotkeyListView = GetDlgItem(g_hSettings[SETTING_HOTKEY], IDC_LIST_HOTKEY);
		ListView_DeleteAllItems(hHotkeyListView);
		for (i=0 ; i<numHotkey ; i++){
			_itoa_s(i, (char *)stri, 3, 5);

			// Running form hotkeys
			GetPrivateProfileString(stri, TEXT("hotkey"), 0, strHotkey, MAX_STRING, strFileName);
			GetPrivateProfileString(stri, TEXT("path"), 0, strPath, MAX_PATH, strFileName);

			InsertListItem(hHotkeyListView, i, strItems, 1, FALSE);
		}
	}

	// Pause hotkey
	GetPrivateProfileString(TEXT("Pause"), TEXT("hotkey"), 0, strHotkey, MAX_STRING, strFileName);
	if (!lstrcmp(strHotkey, ""))
		SendDlgItemMessage(g_hSettings[SETTING_HOTKEY], IDC_HOTKEY_PAUSE, HKM_SETHOTKEY, NULL, 0);
	else
		SendDlgItemMessage(g_hSettings[SETTING_HOTKEY], IDC_HOTKEY_PAUSE, HKM_SETHOTKEY, StringToHotkey(strHotkey), 0);

	return TRUE;
}

BOOL SaveSettings(void){
	UINT	i;
	UINT	numHotkey;
	TCHAR	strFileName[MAX_PATH];
	TCHAR	stri[3];
	TCHAR	str[MAX_STRING];
	HWND	hHotkeyListView;
	LRESULT	key;

	// Get directory
	GetModuleFileName(NULL, strFileName, MAX_PATH);
	PathRemoveFileSpec(strFileName);
	lstrcat(strFileName, TEXT("\\Setting.ini"));

	// Run when windows start
	_itoa_s(SendMessage(GetDlgItem(g_hSettings[SETTING_NORMAL], IDC_CHECK_RUN), BM_GETCHECK, 0, 0), str, MAX_STRING, 10);
	WritePrivateProfileString(TEXT("Run"), TEXT("value"), str, strFileName);
	if (!lstrcmp(str, "1"))
		RegisterRun();
	else
		DeleteRun();

	// Get directory
	PathRemoveFileSpec(strFileName);
	lstrcat(strFileName, TEXT("\\Hotkey.ini"));

	// Running form hotkeys
	hHotkeyListView = GetDlgItem(g_hSettings[SETTING_HOTKEY], IDC_LIST_HOTKEY);
	numHotkey = SendMessage(hHotkeyListView, LVM_GETITEMCOUNT, 0, 0);
	_itoa_s(numHotkey, stri, 3, 5);
	WritePrivateProfileString(TEXT("Hotkey"), TEXT("num"), stri, strFileName);
	for (i=0 ; i<numHotkey ; i++)
	{
		_itoa_s(i, (char *)stri, 3, 5);

		// Hotkey
		ListView_GetItemText(hHotkeyListView, i, 0, str, MAX_STRING);
		WritePrivateProfileString(stri, TEXT("hotkey"), str, strFileName);

		// Path
		ListView_GetItemText(hHotkeyListView, i, 1, str, MAX_STRING);
		WritePrivateProfileString(stri, TEXT("path"), str, strFileName);
	}

	// Pause hotkey
	key = SendDlgItemMessage(g_hSettings[SETTING_HOTKEY], IDC_HOTKEY_PAUSE, HKM_GETHOTKEY, 0, 0);
	HotkeyToString(HIBYTE(LOWORD(key)), (TCHAR)LOBYTE(LOWORD(key)), str);
	WritePrivateProfileString(TEXT("Pause"), TEXT("hotkey"), str, strFileName);

	return TRUE;
}

BOOL RegisterHotkeys(HWND hWnd){
	UINT	i;
	UINT	numHotkey;
	HWND	hHotkeyListView;
	TCHAR	strHotkey[MAX_STRING];
	LRESULT	retKey;
	WORD	wrdKey;

	g_atm = GlobalAddAtom(TEXT("sdHotkey"));
	if (g_atm == NULL)
		return FALSE;

	// Hotkey to run form
	hHotkeyListView = GetDlgItem(g_hSettings[SETTING_HOTKEY], IDC_LIST_HOTKEY);
	numHotkey = SendMessage(hHotkeyListView, LVM_GETITEMCOUNT, 0, 0);
	for (i=0 ; i<numHotkey ; i++)
	{
		ListView_GetItemText(hHotkeyListView, i, 0, strHotkey, MAX_STRING);
		wrdKey = StringToMod(strHotkey);
		RegisterHotKey(hWnd, i, HIBYTE(wrdKey), LOBYTE(wrdKey));
	}

	// Pause
	retKey = SendDlgItemMessage(g_hSettings[SETTING_HOTKEY], IDC_HOTKEY_PAUSE, HKM_GETHOTKEY, 0, 0);
	HotkeyToString(HIBYTE(LOWORD(retKey)), (TCHAR)LOBYTE(LOWORD(retKey)), strHotkey);
	wrdKey = StringToMod(strHotkey);
	RegisterHotKey(hWnd, -1, HIBYTE(wrdKey), LOBYTE(wrdKey));

	return TRUE;
}

BOOL UnregisterHotkeys(HWND hWnd){
	UINT	i;
	UINT	numHotkey;
	HWND	hHotkeyListView;

	// Hotkey to run form
	hHotkeyListView = GetDlgItem(g_hSettings[SETTING_HOTKEY], IDC_LIST_HOTKEY);
	numHotkey = SendMessage(hHotkeyListView, LVM_GETITEMCOUNT, 0, 0);
	for (i=0 ; i<numHotkey ; i++)
		UnregisterHotKey(hWnd, i);

	// Pause
	UnregisterHotKey(hWnd, -1);

	// Delete atom
	GlobalDeleteAtom(g_atm);

	return TRUE;
}

WORD StringToMod(TCHAR string[]){
	BYTE	btMod;
	TCHAR	strPlus[4];
	TCHAR	strMod[8];
	UINT	last;
	UINT	i;

	btMod = 0;
	last = 0;
	lstrcpy(strPlus, "");
	lstrcpy(strMod, "");
	for (i=0 ; i<(UINT)lstrlen(string) ; i++)
	{
		lstrcpyn(strPlus, (LPCSTR)(string + i), 4);
		if (!lstrcmp(strPlus, " + "))
		{
			lstrcpyn(strMod, (LPCSTR)(string + last), i - last + 1);
			if (!lstrcmp(strMod, "Ctrl"))
				btMod = btMod | MOD_CONTROL;
			else if (!lstrcmp(strMod, "Shift"))
				btMod = btMod | MOD_SHIFT;
			else if (!lstrcmp(strMod, "Alt"))
				btMod = btMod | MOD_ALT;

			last = i + 3;
		}
	}

	lstrcpy(strPlus, "");

	return (WORD)MAKEWORD(string[lstrlen(string)-1], btMod);
}

BOOL RegisterRun(void){
	HKEY	hKey;
	DWORD	dwDisp;
	TCHAR	strPath[MAX_PATH];

	// Get directory
	GetModuleFileName(NULL, strPath, MAX_PATH);

	// Register run value
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 14, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hKey, &dwDisp);
	//RegSetKeyValue(hKey, 0, szTitle, REG_SZ, strPath, lstrlen(strPath)); -> error in windowsXP
	RegSetValueEx(hKey, szTitle, 0, REG_SZ, (BYTE*)strPath, lstrlen(strPath));

	// Close register
	RegCloseKey(hKey);

	return TRUE;
}

BOOL DeleteRun(void){
	HKEY	hKey;
	DWORD	dwDisp;
	TCHAR	strPath[MAX_PATH];

	// Get directory
	GetModuleFileName(NULL, strPath, MAX_PATH);

	// Register run value
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 14, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hKey, &dwDisp);
	//RegSetKeyValue(hKey, 0, szTitle, REG_SZ, strPath, lstrlen(strPath));

	// Delete registry
	//RegDeleteKeyValue(hKey, 0, szTitle);	-> error in windowsXP
	RegDeleteValue(hKey, szTitle);

	// Close register
	RegCloseKey(hKey);

	return TRUE;
}