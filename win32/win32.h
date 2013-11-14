#pragma once
#ifndef __SCREENDEVIDER_H
#define __SCREENDEVIDER_H

#define	UM_TRAY WM_USER + 100

#define SETTING_NORMAL	0
#define SETTING_HOTKEY	1
#define SETTING_ABOUT	2

#define SCREENDIVIDER_WIDTH		550
#define SCREENDIVIDER_HEIGHT	247 + 38 + 24 + 12 + 21

#define IDC_BUTTON_OK		201
#define IDC_BUTTON_CANCEL	202
#define IDC_BUTTON_APPLY	203

#define HOTKEY_PAUSE	-1

// Main window
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Tray icon
BOOL CreateTray(HWND hWnd);
BOOL DestroyTray(HWND hWnd);

// setting
BOOL AutoRunProgram(LPSTR lpFileName);
BOOL LoadSettings(void);
BOOL SaveSettings(void);
BOOL RegisterHotkeys(HWND hWnd);
BOOL UnregisterHotkeys(HWND hWnd);
WORD StringToMod(TCHAR string[]);
BOOL RegisterRun(void);
BOOL DeleteRun(void);

// Hook
extern BOOLEAN	HookProc(void);
extern BOOLEAN	UnhookProc(void);
extern BOOLEAN	LoadDataForDll(LPSTR lpFileName);

// Hotkey
extern BOOL CALLBACK dlgSettingHotkeyProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
extern WORD StringToHotkey(TCHAR string[]);

// Common

#endif