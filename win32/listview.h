#pragma once
#ifndef __LISTVIEW_H
#define __LISTVIEW_H

#define NUMITEM 3

LRESULT CALLBACK ListViewProc(HWND hListView, UINT message, WPARAM wParam, LPARAM lParam);
HWND CreateListView(HINSTANCE hInst, HWND hWnd);
BOOL InsertListHeader(HWND hListView, UINT cx, LPWSTR pszHeaderText);
BOOL InsertListItem(HWND hListView, UINT count, LPSTR pszItemText[], UINT num, BOOL isImage);
BOOL ModifyListItem(HWND hListView, UINT count, LPSTR pszItemText[], UINT num, BOOL isImage);

#endif