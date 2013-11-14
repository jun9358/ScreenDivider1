// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#include "resource.h"
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Common control 헤더 파일:
#pragma comment(lib,"comctl32.lib")
#include <commctrl.h>

// ShellAPI 헤더파일 for Tray icon:
#include <ShellAPI.h>

// 프로그램 소스 헤더들:
#include "win32.h"
#include "listview.h"
#include "board.h"
#include "zone.h"
#include "toolbar.h"
#include "finder.h"
#include "transparent.h"
#include "hook.h"
#include "hotkey.h"

// 공용 상수
#define MAX_STRING			255
#define TRANSPARARENT_VALUE	100
#define MIN_WIDTH			303
#define MIN_HEIGHT			223

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
