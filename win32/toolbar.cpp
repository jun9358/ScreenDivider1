#include "stdafx.h"

extern	HINSTANCE	g_hInst;
extern	HWND*		g_hDlgZone;
extern	UINT		g_numZone;
extern	HWND		g_hDlgFinder;
extern	HWND		g_hDlgTransparent;
extern	HWND		g_hDlgBoard;

HWND	g_hDlgToolbar;
HWND	g_hTooltip;
TCHAR	g_formPath[MAX_PATH] = TEXT("");

BOOL CALLBACK dlgToolbarProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	TBBUTTON	ToolBtn[9] = {
		{0, IDC_TOOLBAR_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0},
		{1, IDC_TOOLBAR_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0},
		{2, IDC_TOOLBAR_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0},
		{3, IDC_TOOLBAR_SAVEAS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0},
		{0, IDC_TOOLBAR_SEP, 0, TBSTYLE_SEP, 0, 0, 0, 0},
		{4, IDC_TOOLBAR_FINDER, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0},
		{5, IDC_TOOLBAR_TRANSPARENT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0},
		{0, IDC_TOOLBAR_SEP, 0, TBSTYLE_SEP, 0, 0, 0, 0},
		{6, IDC_TOOLBAR_EXIT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0}
	};
	TCHAR		lpstrFile[MAX_PATH] = TEXT("");

	switch(message)
	{
	case WM_INITDIALOG:
		HWND	hToolbar;

		// Set global variable
		g_hDlgToolbar = hDlg;

		// Create toolbar control
		InitCommonControls();
		hToolbar = CreateToolbarEx(hDlg, WS_CHILD | WS_VISIBLE | WS_BORDER | TBSTYLE_TOOLTIPS,
			ID_TOOLBAR, 7, g_hInst, IDB_TOOLBARITEM, ToolBtn, 9, 16, 16, 16, 16, sizeof(TBBUTTON));

		// Resize dialog
		MoveWindow(hDlg, 0, 0, 183, 53, TRUE);

		return TRUE;

	case WM_COMMAND:
		int				rtnMsg;
		OPENFILENAME	ofn;

		switch(LOWORD(wParam))
		{
		case IDC_TOOLBAR_NEW:
			if (g_numZone != -1)
			{
				rtnMsg = MessageBox(hDlg, TEXT("현재 폼을 저장하시겠습니까?"), TEXT("저장"), MB_YESNO | MB_ICONQUESTION);
				if (rtnMsg == 6)
					SendMessage(hDlg, WM_COMMAND, MAKELPARAM(IDC_TOOLBAR_SAVE, 0), 0);
			}
			NewForm();

			break;
		case IDC_TOOLBAR_OPEN:
			if (g_numZone != -1)
			{
				rtnMsg = MessageBox(hDlg, TEXT("현재 폼을 저장하시겠습니까?"), TEXT("저장"), MB_YESNO | MB_ICONQUESTION);
				if (rtnMsg == 6)
					SendMessage(hDlg, WM_COMMAND, MAKELPARAM(IDC_TOOLBAR_SAVE, 0), 0);
			}

			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFilter = TEXT("Form Files(*.sdfrm)\0*.sdfrm\0");
			ofn.lpstrFile = lpstrFile;
			ofn.nMaxFile = MAX_PATH;
			if (GetOpenFileName(&ofn) == 0)
				break;
			LoadFormFile((LPSTR)lpstrFile);

			break;
		case IDC_TOOLBAR_SAVE:
			if (lstrcmp(g_formPath, TEXT("")) == 0)
			{
				memset(&ofn, 0, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hDlg;
				ofn.lpstrFilter = TEXT("Form Files(*.sdfrm)\0*.sdfrm\0");
				ofn.lpstrFile = lpstrFile;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrTitle = TEXT("저장");
				if (GetSaveFileName(&ofn) == 0)
					break;
				if (SaveFormFile((LPSTR)lpstrFile) == FALSE)
					MessageBox(hDlg, TEXT("폼에 영역이 없어서 저장할 수 없습니다."), TEXT("에러"), MB_ICONERROR);
			}
			else
				if (SaveFormFile((LPSTR)g_formPath) == FALSE)
					MessageBox(hDlg, TEXT("폼에 영역이 없어서 저장할 수 없습니다."), TEXT("에러"), MB_ICONERROR);

			break;
		case IDC_TOOLBAR_SAVEAS:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFilter = TEXT("Form Files(*.sdfrm)\0*.sdfrm\0");
			ofn.lpstrFile = lpstrFile;
			ofn.nMaxFile = MAX_PATH;
			if (GetSaveFileName(&ofn) == 0)
				break;
			if (SaveFormFile((LPSTR)lpstrFile) == FALSE)
				MessageBox(hDlg, TEXT("폼에 영역이 없어서 저장할 수 없습니다."), TEXT("에러"), MB_ICONERROR);

			break;
		case IDC_TOOLBAR_FINDER:
			if (IsWindowVisible(g_hDlgFinder))
				ShowWindow(g_hDlgFinder, SW_HIDE);
			else
				ShowWindow(g_hDlgFinder, SW_SHOW);

			break;
		case IDC_TOOLBAR_TRANSPARENT:
			if (IsWindowVisible(g_hDlgTransparent))
				ShowWindow(g_hDlgTransparent, SW_HIDE);
			else
				ShowWindow(g_hDlgTransparent, SW_SHOW);

			break;
		case IDC_TOOLBAR_EXIT:
			if (g_numZone != -1)
			{
				rtnMsg = MessageBox(hDlg, TEXT("현재 폼을 저장하시겠습니까?"), TEXT("저장"), MB_YESNO | MB_ICONQUESTION);
				if (rtnMsg == 6)
					SendMessage(hDlg, WM_COMMAND, MAKELPARAM(IDC_TOOLBAR_SAVE, 0), 0);
			}

			SaveToolsSettings();

			g_numZone = -1;
			EndDialog(g_hDlgBoard, NULL);
			break;
		}
		
		break;

	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case TTN_GETDISPINFO:
			switch(wParam)
			{
			case IDC_TOOLBAR_NEW:
				((LPNMTTDISPINFO)lParam)->lpszText = "새로 만들기";
				break;
			case IDC_TOOLBAR_OPEN:
				((LPNMTTDISPINFO)lParam)->lpszText = "열기";
				break;
			case IDC_TOOLBAR_SAVE:
				((LPNMTTDISPINFO)lParam)->lpszText = "저장";
				break;
			case IDC_TOOLBAR_SAVEAS:
				((LPNMTTDISPINFO)lParam)->lpszText = "다른 이름으로 저장";
				break;
			case IDC_TOOLBAR_FINDER:
				((LPNMTTDISPINFO)lParam)->lpszText = "파인더";
				break;
			case IDC_TOOLBAR_TRANSPARENT:
				((LPNMTTDISPINFO)lParam)->lpszText = "투명도 설정";
				break;
			case IDC_TOOLBAR_EXIT:
				((LPNMTTDISPINFO)lParam)->lpszText = "종료";
				break;
			}
		}

		break;
	}

	return FALSE;
}

BOOLEAN	NewForm(void){
	int		i;
	UINT	tmpZone;

	// Initialize zone
	tmpZone = g_numZone;
	if (tmpZone != -1)
		for (i=tmpZone ; i>=0 ; i--)
			SendMessage(g_hDlgZone[i], WM_COMMAND, MAKELPARAM(IDC_BUTTON_CLOSE, 0), 0);

	lstrcpy(g_formPath, TEXT(""));

	return TRUE;
}

BOOLEAN	LoadFormFile(LPSTR lpFileName){
	UINT	i;
	TCHAR	stri[3];
	TCHAR	strtmp[MAX_STRING];
	TCHAR	strLeft[5], strTop[5], strRight[5], strBottom[5];
	RECT	crtWnd;
	HWND	hChildDlg;
	HWND	hGroup;
	UINT	numZone, tmpZone;

	// Initialize zone
	tmpZone = g_numZone;
	if (tmpZone != -1)
		for (i=0 ; i<=tmpZone ; i++)
			SendMessage(g_hDlgZone[i], WM_COMMAND, MAKELPARAM(IDC_BUTTON_CLOSE, 0), 0);

	numZone = GetPrivateProfileInt(TEXT("numZone"), TEXT("numZone"), 0, lpFileName);
	for (i=0 ; i<=numZone ; i++)
	{
		_itoa_s(i, (char *)stri, 3, 5);

		hChildDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_ZONE), g_hDlgBoard, dlgZoneProc);
		hGroup = FindWindowEx(hChildDlg, GetDlgItem(hChildDlg, IDC_BUTTON_CLOSE), TEXT("#32770"), TEXT(""));

		// Subject
		GetPrivateProfileString(stri, TEXT("subject"), 0, strtmp, MAX_STRING, lpFileName);
		SetDlgItemText(hGroup, IDC_EDIT_SUBJECT, strtmp);

		// Rectangle
		// GetPrivateProfileInt's return is (-) so use GetPrivateProfileString
		GetPrivateProfileString(stri, TEXT("left"), 0, strLeft, 5, lpFileName);
		GetPrivateProfileString(stri, TEXT("right"), 0, strRight, 5, lpFileName);
		GetPrivateProfileString(stri, TEXT("top"), 0, strTop, 5, lpFileName);
		GetPrivateProfileString(stri, TEXT("bottom"), 0, strBottom, 5, lpFileName);

		SetRect(&crtWnd, atoi((const char*)strLeft), atoi((const char*)strTop), atoi((const char*)strRight), atoi((const char*)strBottom));

		SetDlgItemInt(hGroup, IDC_EDIT_X, crtWnd.left, TRUE);
		SetDlgItemInt(hGroup, IDC_EDIT_Y, crtWnd.top, TRUE);
		SetDlgItemInt(hGroup, IDC_EDIT_WIDTH, crtWnd.right - crtWnd.left, TRUE);
		SetDlgItemInt(hGroup, IDC_EDIT_HEIGHT, crtWnd.bottom - crtWnd.top, TRUE);

		MoveWindow(GetParent(hGroup), crtWnd.left, crtWnd.top, crtWnd.right - crtWnd.left, crtWnd.bottom - crtWnd.top, TRUE);

		// Class name
		GetPrivateProfileString(stri, TEXT("class name"), 0, strtmp, MAX_STRING, lpFileName);
		SetDlgItemText(hGroup, IDC_EDIT_CLASSNAME, strtmp);

		// Path
		GetPrivateProfileString(stri, TEXT("path"), 0, strtmp, MAX_STRING, lpFileName);
		SetDlgItemText(hGroup, IDC_ZONEGROUP_EDIT_PATH, strtmp);

		// Auto run
		SendDlgItemMessage(hGroup, IDC_CHECK_AUTORUN, BM_SETCHECK, GetPrivateProfileInt(stri, TEXT("auto run"), 0, lpFileName), 0);

		// Auto resize
		SendDlgItemMessage(hGroup, IDC_CHECK_AUTORESIZE, BM_SETCHECK, GetPrivateProfileInt(stri, TEXT("auto resize"), 0, lpFileName), 0);
	}

	wsprintf(g_formPath, TEXT("%s"), lpFileName);
	return TRUE;
}
BOOLEAN	SaveFormFile(LPSTR lpFileName){
	UINT	i;
	TCHAR	stri[3];
	TCHAR	strNum[5];
	TCHAR	str[MAX_STRING];
	RECT	crtZone;
	HWND	hZoneGroup;

	if (lstrcmp(lpFileName + lstrlen(lpFileName) - 6, TEXT(".sdfrm")) != 0)
		lstrcat(lpFileName, TEXT(".sdfrm"));

	if (g_numZone == -1)
		return FALSE;

	for(i=0 ; i<=g_numZone ; i++)
	{
		_itoa_s(i, (char *)stri, 3, 10);
		hZoneGroup = FindWindowEx(g_hDlgZone[i], GetDlgItem(g_hDlgZone[i], IDC_BUTTON_CLOSE), TEXT("#32770"), TEXT(""));

		// numZone
		_itoa_s(g_numZone, (char *)strNum, 5, 10);
		WritePrivateProfileString(TEXT("numZone"), TEXT("numZone"), (LPSTR)strNum, lpFileName);

		// Subject
		GetDlgItemText(hZoneGroup, IDC_EDIT_SUBJECT, str, MAX_STRING);
		WritePrivateProfileString(stri, TEXT("subject"), str, lpFileName);

		// Rectangle
		GetWindowRect(g_hDlgZone[i], &crtZone);
		_itoa_s(crtZone.bottom, (char *)strNum, 5, 10);
		WritePrivateProfileString(stri, TEXT("bottom"), strNum, lpFileName);
		_itoa_s(crtZone.left, (char *)strNum, 5, 10);
		WritePrivateProfileString(stri, TEXT("left"), strNum, lpFileName);
		_itoa_s(crtZone.right, (char *)strNum, 5, 10);
		WritePrivateProfileString(stri, TEXT("right"), strNum, lpFileName);
		_itoa_s(crtZone.top, (char *)strNum, 5, 10);
		WritePrivateProfileString(stri, TEXT("top"), strNum, lpFileName);

		// Class name
		GetDlgItemText(hZoneGroup, IDC_EDIT_CLASSNAME, str, MAX_STRING);
		WritePrivateProfileString(stri, TEXT("class name"), str, lpFileName);

		// Path
		GetDlgItemText(hZoneGroup, IDC_EDIT_FILEPATH, str, MAX_STRING);
		WritePrivateProfileString(stri, TEXT("path"), str, lpFileName);

		// Auto run
		_itoa_s(SendMessage(GetDlgItem(hZoneGroup, IDC_CHECK_AUTORUN), BM_GETCHECK, 0, 0), (char *)strNum, 5, 10);
		WritePrivateProfileString(stri, TEXT("auto run"), strNum, lpFileName);

		// Auto resize
		_itoa_s(SendMessage(GetDlgItem(hZoneGroup, IDC_CHECK_AUTORESIZE), BM_GETCHECK, 0, 0), (char *)strNum, 5, 10);
		WritePrivateProfileString(stri, TEXT("auto resize"), strNum, lpFileName);
	}

	wsprintf(g_formPath, TEXT("%s"), lpFileName);

	return TRUE;
}