#pragma once
#ifndef __BOARD_H
#define __BOARD_H

#pragma comment(lib, "shlwapi.lib")
#include <shlwapi.h>
#include <CommDlg.h>
#pragma comment(lib, "psapi.lib")
#include <psapi.h>

#define FILETYPE	".sdfrm"

#define HOTKEY_ID_NEW			0
#define HOTKEY_ID_OPEN			1
#define HOTKEY_ID_SAVE			2
#define HOTKEY_ID_FINDER		3
#define HOTKEY_ID_TRANSPARENT	4
#define HOTKEY_ID_HIDESHOW		5

#define HOTKEY_VK_NEW			'N'
#define HOTKEY_VK_OPEN			'O'
#define HOTKEY_VK_SAVE			'S'
#define HOTKEY_VK_FIDNER		'F'
#define HOTKEY_VK_TRANSPARENT	'T'
#define HOTKEY_VK_HIDESHOW		VK_TAB

#define HOTKEY_MOD_NEW			MOD_CONTROL
#define HOTKEY_MOD_OPEN			MOD_CONTROL
#define HOTKEY_MOD_SAVE			MOD_CONTROL
#define HOTKEY_MOD_FINDER		MOD_CONTROL
#define HOTKEY_MOD_TRANSPARENT	MOD_CONTROL
#define HOTKEY_MOD_HIDESHOW		NULL

#define VISIBLE_FINDER			1
#define VISIBLE_TRANSPARENT		2

BOOL CALLBACK dlgBoardProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOLEAN	LoadToolsSetting(void);
BOOLEAN SaveToolsSettings(void);

extern BOOL CALLBACK dlgToolbarProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlgFinderProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlgTransparentProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlgZoneProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
extern BOOLEAN	LoadSubjectList(void);

#endif