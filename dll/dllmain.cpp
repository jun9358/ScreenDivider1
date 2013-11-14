// dllmain.cpp : DLL 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"

HINSTANCE	g_hDllInst;

extern	HWND		g_hTarget;
extern	LONG		g_lpPrevMessageProc;
extern	DATZONE*	g_datZone;

BOOL APIENTRY DllMain( HINSTANCE hInstance,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hDllInst = hInstance;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		// Release memory
		UnmapViewOfFile(g_datZone);

		// Unhook
		SetWindowLong(g_hTarget, GWL_WNDPROC, g_lpPrevMessageProc);

		break;
	}
	return TRUE;
}