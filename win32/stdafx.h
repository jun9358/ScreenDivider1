// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#include "resource.h"
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Common control ��� ����:
#pragma comment(lib,"comctl32.lib")
#include <commctrl.h>

// ShellAPI ������� for Tray icon:
#include <ShellAPI.h>

// ���α׷� �ҽ� �����:
#include "win32.h"
#include "listview.h"
#include "board.h"
#include "zone.h"
#include "toolbar.h"
#include "finder.h"
#include "transparent.h"
#include "hook.h"
#include "hotkey.h"

// ���� ���
#define MAX_STRING			255
#define TRANSPARARENT_VALUE	100
#define MIN_WIDTH			303
#define MIN_HEIGHT			223

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
