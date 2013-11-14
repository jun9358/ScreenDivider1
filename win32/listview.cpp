#include "stdafx.h"

LPSTR lpszListItemName[NUMITEM] = {TEXT("일　반"), TEXT("단축키(핫키)"), TEXT("정　보")};
WNDPROC	procListView;

HWND CreateListView(HINSTANCE hInst, HWND hWnd){
	HIMAGELIST	hMenuIcon;
	HWND		hListView;

	// Crate list view control.
	InitCommonControls();
	hListView = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
		WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | 
		LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT | LVS_NOCOLUMNHEADER, 12, 12, 130, 247, hWnd, NULL, hInst, NULL);
	ListView_SetExtendedListViewStyle((HWND)hListView, LVS_EX_FULLROWSELECT);
	hMenuIcon = ImageList_LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LISTITE), 20, 1, RGB(255, 255, 255));
	SendMessage(hListView, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)hMenuIcon);

	// initialize list view control.
	InsertListHeader((HWND)hListView, 125, NULL);
	InsertListItem((HWND)hListView, 0, lpszListItemName + 0, 0, TRUE);
	InsertListItem((HWND)hListView, 1, lpszListItemName + 1, 0, TRUE);
	InsertListItem((HWND)hListView, 2, lpszListItemName + 2, 0, TRUE);

	return hListView;
}

BOOL InsertListHeader(HWND hListView, UINT cx, LPWSTR pszHeaderText){
	LVCOLUMN	lvCol;

	lvCol.mask		= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt		= LVCFMT_LEFT;
	lvCol.cx		= cx;
	lvCol.pszText	= (LPSTR)pszHeaderText;
	lvCol.iSubItem	= 0;
	SendMessage(hListView, LVM_INSERTCOLUMN, 0, (LPARAM)&lvCol);

	return TRUE;
}

BOOL InsertListItem(HWND hListView, UINT count, LPSTR pszItemText[], UINT num, BOOL isImage){
	LVITEM	lvItem;
	UINT	i;

	lvItem.mask		= LVIF_TEXT | LVIF_IMAGE;
	if (isImage == TRUE)
		lvItem.iImage	= count;
	else
		lvItem.iImage	= -1;
	lvItem.iSubItem	= 0;
	lvItem.iItem	= count;
	lvItem.pszText	= (LPSTR)pszItemText[0];
	SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

	for (i=1 ; i<=num ; i++)
	{
		lvItem.iImage	= -1;
		lvItem.iSubItem	= i;
		lvItem.pszText	= (LPSTR)pszItemText[i];
		SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);
	}

	return TRUE;
}

BOOL ModifyListItem(HWND hListView, UINT count, LPSTR pszItemText[], UINT num, BOOL isImage){
	LVITEM	lvItem;
	UINT	i;

	lvItem.mask		= LVIF_TEXT | LVIF_IMAGE;
	if (isImage == TRUE)
		lvItem.iImage	= count;
	else
		lvItem.iImage	= -1;
	lvItem.iSubItem	= 0;
	lvItem.iItem	= count;
	lvItem.pszText	= (LPSTR)pszItemText[0];
	SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

	for (i=1 ; i<=num ; i++)
	{
		lvItem.iImage	= -1;
		lvItem.iSubItem	= i;
		lvItem.pszText	= (LPSTR)pszItemText[i];
		SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);
	}

	return TRUE;
}