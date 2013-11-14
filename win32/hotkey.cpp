#include "stdafx.h"

extern	HINSTANCE	g_hInst;

HWND	g_hDlgSettingHotkey;
HWND	g_hHotkeyListView;
HWND	g_hDlgHotkeyEdit;
UINT	g_EditIndex;

BOOL CALLBACK dlgSettingHotkeyProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message)
	{
	case WM_INITDIALOG:
		g_hDlgSettingHotkey = hDlg;

		g_hHotkeyListView = GetDlgItem(hDlg, IDC_LIST_HOTKEY);
		ListView_SetExtendedListViewStyle((HWND)g_hHotkeyListView, LVS_EX_FULLROWSELECT);

		InsertListHeader(g_hHotkeyListView, 190, (LPWSTR)"폼 경로");
		InsertListHeader(g_hHotkeyListView, 157, (LPWSTR)"단축키");

		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			g_EditIndex = -1;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SETTING_HOTKEY_EDIT), hDlg, dlgSettingHotkeyEditProc);

			break;
		case IDC_BUTTON_MODIFY:
			g_EditIndex = ListView_GetNextItem(g_hHotkeyListView, -1, LVNI_ALL | LVNI_SELECTED);
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SETTING_HOTKEY_EDIT), hDlg, dlgSettingHotkeyEditProc);

			break;
		case IDC_BUTTON_DELETE:
			ListView_DeleteItem(g_hHotkeyListView, ListView_GetNextItem(g_hHotkeyListView, -1, LVNI_ALL | LVNI_SELECTED));

			break;
		}
	}

	return FALSE;
}

BOOL CALLBACK dlgSettingHotkeyEditProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message)
	{
	case WM_INITDIALOG:
		TCHAR	str[MAX_STRING];

		g_hDlgHotkeyEdit = hDlg;

		if (g_EditIndex != -1)
		{
			ListView_GetItemText(g_hHotkeyListView, g_EditIndex, 0, str, MAX_STRING);
			SendDlgItemMessage(hDlg, IDC_HOTKEY_RUN_FORM, HKM_SETHOTKEY, StringToHotkey(str), 0);
			ListView_GetItemText(g_hHotkeyListView, g_EditIndex, 1, str, MAX_STRING);
			SetDlgItemText(hDlg, IDC_EDIT_PATH_FORM, str);
		}

		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			LPSTR	strItems[2];
			TCHAR	strHotkey[MAX_STRING];
			TCHAR	strPath[MAX_STRING];
			LRESULT	key;

			strItems[0] = strHotkey;
			strItems[1] = strPath;
			key = SendDlgItemMessage(hDlg, IDC_HOTKEY_RUN_FORM, HKM_GETHOTKEY, 0, 0);
			HotkeyToString(HIBYTE(LOWORD(key)), (TCHAR)LOBYTE(LOWORD(key)), strHotkey);
			GetDlgItemText(hDlg, IDC_EDIT_PATH_FORM, strPath, MAX_STRING);
			if (g_EditIndex == -1)
				InsertListItem(g_hHotkeyListView, SendDlgItemMessage(g_hDlgSettingHotkey, IDC_LIST_HOTKEY, LVM_GETITEMCOUNT, 0, 0), strItems, 1, FALSE);
			else
				ModifyListItem(g_hHotkeyListView, g_EditIndex, strItems, 1, FALSE);

			// EndDialog(hDlg, IDCANCEL); -> case IDCANCEL;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);

			break;
		case IDC_BUTTON_FILE_OPEN:
			OPENFILENAME	ofn;
			TCHAR			lpstrFile[MAX_PATH] = TEXT("");

			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFilter = TEXT("Form Files(*.sdfrm)\0*.sdfrm\0");
			ofn.lpstrFile = lpstrFile;
			ofn.nMaxFile = MAX_PATH;
			if (GetOpenFileName(&ofn) == 0)
				break;
			SetDlgItemText(hDlg, IDC_EDIT_PATH_FORM, lpstrFile);
		}
	}

	return FALSE;
}

BOOLEAN	HotkeyToString(WORD mod, TCHAR vk, LPSTR string){
	TCHAR	strMod[32];

	lstrcpy(strMod, "");
	if (mod & HOTKEYF_CONTROL)
		lstrcat(strMod, "Ctrl + ");
	if (mod & HOTKEYF_SHIFT)
		lstrcat(strMod, "Shift + ");
	if (mod & HOTKEYF_ALT)
		lstrcat(strMod, "Alt + ");
	if (mod & HOTKEYF_EXT)
		lstrcat(strMod, "Ext + ");
	
	if (vk == 0)
	{
		lstrcpy(string, "None");
		return FALSE;
	}
	wsprintf(string, "%s%c", strMod, vk);

	return TRUE;
}

WORD StringToHotkey(TCHAR string[]){
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
				btMod = btMod | HOTKEYF_CONTROL;
			else if (!lstrcmp(strMod, "Shift"))
				btMod = btMod | HOTKEYF_SHIFT;
			else if (!lstrcmp(strMod, "Alt"))
				btMod = btMod | HOTKEYF_ALT;
			else if (!lstrcmp(strMod, "Ext"))
				btMod = btMod | HOTKEYF_EXT;

			last = i + 3;
		}
	}

	lstrcpy(strPlus, "");

	return (WORD)MAKEWORD(string[lstrlen(string)-1], btMod);
}