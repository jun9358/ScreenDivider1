#include "stdafx.h"

extern	HINSTANCE	g_hInst;
extern	HWND*	g_hDlgZone;
extern	UINT	g_numZone;
extern	HWND	g_hDlgBoard;

HWND	g_hDlgFinder;
BOOL	g_isFinder;
WNDPROC	g_hHook;

BOOL CALLBACK dlgFinderProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message)
	{
	case WM_INITDIALOG:
		// Set finder dialog handle
		g_hDlgFinder = hDlg;

		// Hook the proc function for finder control
		g_hHook = (WNDPROC)GetWindowLong(GetDlgItem(hDlg, IDC_PICTURE_FINDER), GWL_WNDPROC);
		SetWindowLong(GetDlgItem(hDlg, IDC_PICTURE_FINDER), GWL_WNDPROC, (LONG)finderProc);

		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_SYNC:
			HWND	hGroup;
			TCHAR	strClassname[MAX_CLASS_NAME];
			TCHAR	strX[5], strY[5], strWidth[5], strHeight[5];

			hGroup = FindWindowEx(g_hDlgZone[SendDlgItemMessage(hDlg, IDC_COMBO_SUBJECT, CB_GETCURSEL, 0, 0)], GetDlgItem(hDlg, IDC_BUTTON_CLOSE), TEXT("#32770"), TEXT(""));

			// Class name
			GetDlgItemText(hDlg, IDC_STATIC_CLASSNAME, strClassname, MAX_CLASS_NAME);
			SetDlgItemText(hGroup, IDC_EDIT_CLASSNAME, strClassname);

			// Path
			GetDlgItemText(hDlg, IDC_FINDER_STATIC_PATH, strClassname, MAX_CLASS_NAME);
			SetDlgItemText(hGroup, IDC_ZONEGROUP_EDIT_PATH, strClassname);

			// Rectangle
			// GetDlgItemInt's return is (-) so use GetDlgItemText
			GetDlgItemText(hDlg, IDC_STATIC_X, strX, 5);
			GetDlgItemText(hDlg, IDC_STATIC_Y, strY, 5);
			GetDlgItemText(hDlg, IDC_STATIC_WIDTH, strWidth, 5);
			GetDlgItemText(hDlg, IDC_STATIC_HEIGHT, strHeight, 5);

			SetDlgItemText(hGroup, IDC_EDIT_X, strX);
			SetDlgItemText(hGroup, IDC_EDIT_Y, strY);
			SetDlgItemText(hGroup, IDC_EDIT_WIDTH, strWidth);
			SetDlgItemText(hGroup, IDC_EDIT_HEIGHT, strHeight);

			MoveWindow(GetParent(hGroup), atoi((const char*)strX), atoi((const char*)strY), atoi((const char*)strWidth), atoi((const char*)strHeight), TRUE);

			break;
		case IDCANCEL:
			// Hide dialog
			ShowWindow(hDlg, SW_HIDE);

			break;
		}

		return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK finderProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	POINT	pt;

	switch(message)
	{
	case WM_LBUTTONDOWN:
		ShowWindow(g_hDlgBoard, SW_HIDE);
		SetCursor(LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_FINDER)));
		g_isFinder = TRUE;

		SetCapture(hDlg);

		break;
	case WM_MOUSEMOVE:
		HWND			hFinded;
		TCHAR			str[MAX_STRING];
		unsigned long	pid;
		HANDLE			hProcess;
		RECT			crtFinded;

		if (g_isFinder == FALSE)
			break;

		// Get current cursor position
		GetCursorPos(&pt);

		// Get window under the position
		hFinded = WindowFromPoint(pt);
		if (!IsWindow(hFinded))
			break;

		// Get most top parent window
		while (GetParent(hFinded))
		{
			hFinded = GetParent(hFinded);
		}

		// Get caption
		GetWindowText(hFinded, str, MAX_STRING);
		// Print caption
		SetDlgItemText(g_hDlgFinder, IDC_STATIC_CAPTION, str);

		// Get class
		GetClassName(hFinded, str, MAX_CLASS_NAME);
		// Print class
		SetDlgItemText(g_hDlgFinder, IDC_STATIC_CLASSNAME, str);

		// Get rect
		GetWindowRect(hFinded, &crtFinded);
		// Print rect
		SetDlgItemInt(g_hDlgFinder, IDC_STATIC_X, crtFinded.left, TRUE);
		SetDlgItemInt(g_hDlgFinder, IDC_STATIC_Y, crtFinded.top, TRUE);
		SetDlgItemInt(g_hDlgFinder, IDC_STATIC_WIDTH, crtFinded.right - crtFinded.left, TRUE);
		SetDlgItemInt(g_hDlgFinder, IDC_STATIC_HEIGHT, crtFinded.bottom - crtFinded.top, TRUE);

		// Get process id
		GetWindowThreadProcessId(hFinded, &pid);

		// Get process handle
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (hProcess == 0)
			break;

		// Get filename
		GetModuleFileNameEx(hProcess, NULL, str, MAX_PATH);
		// Print filename
		SetDlgItemText(g_hDlgFinder, IDC_FINDER_STATIC_PATH, str);

		break;
	case WM_LBUTTONUP:
		// Initialize finder cursor
		if (g_isFinder == FALSE)
			break;
		g_isFinder = FALSE;
		ShowWindow(g_hDlgBoard, SW_SHOW);
		ReleaseCapture();
	}
	return CallWindowProc(g_hHook, hDlg, message, wParam, lParam);
}

BOOLEAN	LoadSubjectList(void){
	UINT	i;
	HWND	hGroup;
	TCHAR	strSubject[MAX_STRING];

	SendDlgItemMessage(g_hDlgFinder, IDC_COMBO_SUBJECT, CB_RESETCONTENT, 0, 0);

	if (g_numZone == -1)
		return FALSE;

	for (i=0 ; i<=g_numZone ; i++)
	{
		hGroup = FindWindowEx(g_hDlgZone[i], GetDlgItem(g_hDlgZone[i], IDC_BUTTON_CLOSE), TEXT("#32770"), TEXT(""));
		GetDlgItemText(hGroup, IDC_EDIT_SUBJECT, strSubject, MAX_STRING);
		SendDlgItemMessage(g_hDlgFinder, IDC_COMBO_SUBJECT, CB_ADDSTRING, 0, (LPARAM)strSubject);
	}
	SendDlgItemMessage(g_hDlgFinder, IDC_COMBO_SUBJECT, CB_SETCURSEL, 0, 0);

	return TRUE;
}