BOOL CALLBACK dlgSettingHotkeyProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dlgSettingHotkeyEditProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOLEAN	HotkeyToString(WORD mod, TCHAR vk, LPSTR string);
WORD StringToHotkey(TCHAR string[]);

// listview.cpp
extern BOOL InsertListHeader(HWND hListView, UINT cx, LPWSTR pszHeaderText);
extern BOOL InsertListItem(HWND hListView, UINT count, LPSTR pszItemText[], UINT num, BOOL isImage);
extern BOOL ModifyListItem(HWND hListView, UINT count, LPSTR pszItemText[], UINT num, BOOL isImage);