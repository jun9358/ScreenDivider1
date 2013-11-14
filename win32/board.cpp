#include "stdafx.h"

extern	HINSTANCE	g_hInst;
extern	HWND		g_hDlgToolbar;
extern	HWND*		g_hDlgZone;
extern	HWND		g_hDlgFinder;
extern	HWND		g_hDlgTransparent;
extern	UINT		g_numZone;
extern	TCHAR		g_formPath[MAX_PATH];

HWND	g_hDlgBoard;
LONG	g_midX, g_midY;
BOOL	g_isBoardDrag;
UINT	stsVisible;

BOOL CALLBACK dlgBoardProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	POINT	pt;
	HWND	hChildDlg;

	switch(message)
	{
	case WM_HOTKEY:
		switch(wParam)
		{
		case HOTKEY_ID_NEW:
			SendMessage(g_hDlgToolbar, WM_COMMAND, IDC_TOOLBAR_NEW, 0);

			break;
		case HOTKEY_ID_OPEN:
			SendMessage(g_hDlgToolbar, WM_COMMAND, IDC_TOOLBAR_OPEN, 0);

			break;
		case HOTKEY_ID_SAVE:
			SendMessage(g_hDlgToolbar, WM_COMMAND, IDC_TOOLBAR_SAVE, 0);

			break;
		case HOTKEY_ID_FINDER:
			SendMessage(g_hDlgToolbar, WM_COMMAND, IDC_TOOLBAR_FINDER, 0);

			break;
		case HOTKEY_ID_TRANSPARENT:
			SendMessage(g_hDlgToolbar, WM_COMMAND, IDC_TOOLBAR_TRANSPARENT, 0);

			break;
		case HOTKEY_ID_HIDESHOW:
			if (IsWindowVisible(g_hDlgToolbar)){
				// Set visible status
				stsVisible = 0;
				if (IsWindowVisible(g_hDlgFinder))
					stsVisible += VISIBLE_FINDER;
				if (IsWindowVisible(g_hDlgTransparent))
					stsVisible += VISIBLE_TRANSPARENT;

				ShowWindow(g_hDlgToolbar, SW_HIDE);
				ShowWindow(g_hDlgFinder, SW_HIDE);
				ShowWindow(g_hDlgTransparent, SW_HIDE);
			}
			else{
				// Get visible status and show dialogs
				switch(stsVisible)
				{
				case VISIBLE_FINDER:
					ShowWindow(g_hDlgFinder, SW_SHOW);
					break;
				case VISIBLE_TRANSPARENT:
					ShowWindow(g_hDlgTransparent, SW_SHOW);
					break;
				case VISIBLE_FINDER + VISIBLE_TRANSPARENT:
					ShowWindow(g_hDlgFinder, SW_SHOW);
					ShowWindow(g_hDlgTransparent, SW_SHOW);
					break;
				}
				stsVisible = 0;
				ShowWindow(g_hDlgToolbar, SW_SHOW);
			}

			break;
		}

		break;

	case WM_INITDIALOG:
		// Set handle
		g_hDlgBoard = hDlg;

		// Resize to screen size
		MoveWindow(hDlg, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE);

		// Set transparent attitude
		SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hDlg, 0, TRANSPARARENT_VALUE, LWA_ALPHA);

		// Create toolbar dialog
		hChildDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_TOOLBAR), hDlg, dlgToolbarProc);

		// Create finder dialog
		hChildDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_FINDER), hDlg, dlgFinderProc);

		// Create transparent dialog
		hChildDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_TRANSPARENT), hDlg, dlgTransparentProc);

		// Load tools setting
		LoadToolsSetting();

		// Register hotkey
		RegisterHotKey(hDlg, HOTKEY_ID_NEW, HOTKEY_MOD_NEW, HOTKEY_VK_NEW);
		RegisterHotKey(hDlg, HOTKEY_ID_OPEN, HOTKEY_MOD_OPEN, HOTKEY_VK_OPEN);
		RegisterHotKey(hDlg, HOTKEY_ID_SAVE, HOTKEY_MOD_SAVE, HOTKEY_VK_SAVE);
		RegisterHotKey(hDlg, HOTKEY_ID_FINDER, HOTKEY_MOD_FINDER, HOTKEY_VK_FIDNER);
		RegisterHotKey(hDlg, HOTKEY_ID_TRANSPARENT, HOTKEY_MOD_TRANSPARENT, HOTKEY_VK_TRANSPARENT);
		RegisterHotKey(hDlg, HOTKEY_ID_HIDESHOW, HOTKEY_MOD_HIDESHOW, HOTKEY_VK_HIDESHOW);

		if (lstrcmp(g_formPath, ""))
			LoadFormFile(g_formPath);

		// Show toolbar dialog
		ShowWindow(g_hDlgToolbar, SW_SHOW);

		return TRUE;
	case WM_LBUTTONDOWN:
		// Drag setting
		g_isBoardDrag = TRUE;

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// Create zone dialog(modeless) and move
		hChildDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_ZONE), hDlg, dlgZoneProc);
		ShowWindow(hChildDlg, SW_SHOW);
		MoveWindow(hChildDlg, pt.x, pt.y, 0, 0, TRUE);

		// Set clicked point for drawing zone
		g_midX = pt.x;
		g_midY = pt.y;

		// Set capture for outing control
		SetCapture(hDlg);
		return TRUE;
	case WM_MOUSEMOVE:
		RECT	crtDlg;

		// Drag...?
		if(g_isBoardDrag == FALSE)
			return TRUE;

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// Backup window rect to redraw window
		GetWindowRect(g_hDlgZone[g_numZone], &crtDlg);

		// Resize dialog to drawing size
		if(pt.x - g_midX < 0 && pt.y - g_midY < 0){
			MoveWindow(g_hDlgZone[g_numZone], pt.x, pt.y, g_midX - pt.x, g_midY - pt.y, TRUE);
		}
		else if(pt.x - g_midX > 0 && pt.y - g_midY < 0){
			MoveWindow(g_hDlgZone[g_numZone], g_midX, pt.y, pt.x - g_midX, g_midY - pt.y, TRUE);
		}
		else if(pt.x - g_midX > 0 && pt.y - g_midY > 0){
			MoveWindow(g_hDlgZone[g_numZone], g_midX, g_midY, pt.x - g_midX, pt.y - g_midY, TRUE);
		}
		else{// pt.x - midX < 0 && pt.y - midY > 0
			MoveWindow(g_hDlgZone[g_numZone], pt.x, g_midY, g_midX - pt.x, pt.y - g_midY, TRUE);
		}

		//Redraw
		InvalidateRect(hDlg, &crtDlg, TRUE);

		return TRUE;
	case WM_LBUTTONUP:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// Drag...?
		if(g_isBoardDrag == FALSE)
			return TRUE;

		// Min size			
		if (abs(pt.x - g_midX) < MIN_WIDTH || abs(pt.y - g_midY) < MIN_HEIGHT)
		{
			MessageBox(hDlg, TEXT("너무 작은 영역입니다."), TEXT("에러"), MB_ICONERROR);
			EndDialog(g_hDlgZone[g_numZone], NULL);
			g_numZone -= 1;
			LoadSubjectList();
		}

		// Release capture for outing control
		ReleaseCapture();

		// Initialize drag setting
		g_isBoardDrag = FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOLEAN	LoadToolsSetting(void){
	TCHAR	strFileName[MAX_PATH];
	RECT	crtWnd;

	// Get directory
	GetModuleFileName(NULL, strFileName, MAX_PATH);
	PathRemoveFileSpec(strFileName);
	lstrcat(strFileName, TEXT("\\Setting.ini"));

	if (PathFileExists(strFileName) == FALSE)
		return FALSE;

	// Toolbar
	SetRect(&crtWnd, GetPrivateProfileInt(TEXT("Toolbar"), TEXT("left"), 0, strFileName), GetPrivateProfileInt(TEXT("Toolbar"), TEXT("top"), 0, strFileName),
		GetPrivateProfileInt(TEXT("Toolbar"), TEXT("right"), 0, strFileName), GetPrivateProfileInt(TEXT("Toolbar"), TEXT("bottom"), 0, strFileName));
	MoveWindow(g_hDlgToolbar, crtWnd.left, crtWnd.top, crtWnd.right - crtWnd.left, crtWnd.bottom - crtWnd.top, TRUE);

	// Finder
	SetRect(&crtWnd, GetPrivateProfileInt(TEXT("Finder"), TEXT("left"), 0, strFileName), GetPrivateProfileInt(TEXT("Finder"), TEXT("top"), 0, strFileName),
		GetPrivateProfileInt(TEXT("Finder"), TEXT("right"), 0, strFileName), GetPrivateProfileInt(TEXT("Finder"), TEXT("bottom"), 0, strFileName));
	MoveWindow(g_hDlgFinder, crtWnd.left, crtWnd.top, crtWnd.right - crtWnd.left, crtWnd.bottom - crtWnd.top, TRUE);
	if (GetPrivateProfileInt(TEXT("Finder"), TEXT("visible"), 0, strFileName))
		ShowWindow(g_hDlgFinder, SW_SHOW);
	else
		ShowWindow(g_hDlgFinder, SW_HIDE);

	// Transparent
	SetRect(&crtWnd, GetPrivateProfileInt(TEXT("Transparent"), TEXT("left"), 0, strFileName), GetPrivateProfileInt(TEXT("Transparent"), TEXT("top"), 0, strFileName),
		GetPrivateProfileInt(TEXT("Transparent"), TEXT("right"), 0, strFileName), GetPrivateProfileInt(TEXT("Transparent"), TEXT("bottom"), 0, strFileName));
	MoveWindow(g_hDlgTransparent, crtWnd.left, crtWnd.top, crtWnd.right - crtWnd.left, crtWnd.bottom - crtWnd.top, TRUE);
	if (GetPrivateProfileInt(TEXT("Transparent"), TEXT("visible"), 0, strFileName))
		ShowWindow(g_hDlgTransparent, SW_SHOW);
	else
		ShowWindow(g_hDlgTransparent, SW_HIDE);
	SendDlgItemMessage(g_hDlgTransparent, IDC_SLIDER_TRANSPARENT, TBM_SETPOS, TRUE, GetPrivateProfileInt(TEXT("Transparent"), TEXT("value"), 0, strFileName));
	SetLayeredWindowAttributes(g_hDlgBoard, 0, GetPrivateProfileInt(TEXT("Transparent"), TEXT("value"), 0, strFileName), LWA_ALPHA);

	return TRUE;
}
BOOLEAN SaveToolsSettings(void){
	TCHAR	strFileName[MAX_PATH];
	RECT	crtWnd;
	TCHAR	strRect[5];

	// Get directory
	GetModuleFileName(NULL, strFileName, MAX_PATH);
	PathRemoveFileSpec(strFileName);
	lstrcat(strFileName, TEXT("\\Setting.ini"));

	// Toolbar
	GetWindowRect(g_hDlgToolbar, &crtWnd);
	_itoa_s(crtWnd.left, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Toolbar"), TEXT("left"), strRect, strFileName);
	_itoa_s(crtWnd.right, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Toolbar"), TEXT("right"), strRect, strFileName);
	_itoa_s(crtWnd.top, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Toolbar"), TEXT("top"), strRect, strFileName);
	_itoa_s(crtWnd.bottom, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Toolbar"), TEXT("bottom"), strRect, strFileName);

	// Finder
	if (IsWindowVisible(g_hDlgFinder))
		WritePrivateProfileString(TEXT("Finder"), TEXT("visible"), TEXT("1"), strFileName);
	else
		WritePrivateProfileString(TEXT("Finder"), TEXT("visible"), TEXT("0"), strFileName);
	GetWindowRect(g_hDlgFinder, &crtWnd);
	_itoa_s(crtWnd.left, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Finder"), TEXT("left"), strRect, strFileName);
	_itoa_s(crtWnd.right, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Finder"), TEXT("right"), strRect, strFileName);
	_itoa_s(crtWnd.top, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Finder"), TEXT("top"), strRect, strFileName);
	_itoa_s(crtWnd.bottom, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Finder"), TEXT("bottom"), strRect, strFileName);

	// Transparent
	if (IsWindowVisible(g_hDlgTransparent))
		WritePrivateProfileString(TEXT("Transparent"), TEXT("visible"), TEXT("1"), strFileName);
	else
		WritePrivateProfileString(TEXT("Transparent"), TEXT("visible"), TEXT("0"), strFileName);
	GetWindowRect(g_hDlgTransparent, &crtWnd);
	_itoa_s(crtWnd.left, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Transparent"), TEXT("left"), strRect, strFileName);
	_itoa_s(crtWnd.right, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Transparent"), TEXT("right"), strRect, strFileName);
	_itoa_s(crtWnd.top, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Transparent"), TEXT("top"), strRect, strFileName);
	_itoa_s(crtWnd.bottom, (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Transparent"), TEXT("bottom"), strRect, strFileName);
	_itoa_s(SendDlgItemMessage(g_hDlgTransparent, IDC_SLIDER_TRANSPARENT, TBM_GETPOS, 0, 0), (char *)strRect, 5, 10);
	WritePrivateProfileString(TEXT("Transparent"), TEXT("value"), strRect, strFileName);

	return TRUE;
}