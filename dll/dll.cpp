// dll.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//
/************************************************************************/
/* Export Functions                                                     */
/************************************************************************/
#include "stdafx.h"

#pragma data_seg("SHAREDATA")
UINT		g_numZone = -1;
TCHAR		g_hFilePath[MAX_PATH] = TEXT("");
#pragma data_seg()
#pragma comment(linker, "/SECTION:SHAREDATA,RWS")

extern	HINSTANCE	g_hDllInst;

HHOOK		g_hHookProc;
LONG		g_lpPrevMessageProc = NULL;
HWND		g_hTarget;
DATZONE*	g_datZone = NULL;
UINT		g_hCreatedIndex = -1;

LRESULT CALLBACK hookMinMaxProc(int nCode, WPARAM wParam, LPARAM lParam){
	if(nCode < 0)
		return CallNextHookEx(g_hHookProc, nCode, wParam, lParam);

	switch(nCode)
	{
	case HCBT_MINMAX:
		// Hook message
		if (lParam == SW_SHOWMAXIMIZED)
		{
			if (g_lpPrevMessageProc != NULL)
				break;

			g_hTarget		= (HWND)wParam;
			g_lpPrevMessageProc = SetWindowLong(g_hTarget, GWL_WNDPROC, (LONG)hookMessageProc);
		}

		break;

	case HCBT_CREATEWND:
		TCHAR			str[MAX_STRING];
		unsigned long	pid;
		HANDLE			hProcess;
		UINT			i;

		if (g_numZone == -1)
			break;

		// Read file for sharing memory
		g_datZone = (DATZONE*)ReadFileMapping(ZONEMAPNAME);
		if (g_datZone == NULL){
			MessageBox((HWND)wParam, "Can't open mapping file!!!", "Error", MB_OK);

			break;
		}

		for (i=0 ; i<=g_numZone ; i++)
		{
			if (g_datZone[i].isAutoResize == TRUE)
			{
				// Get class name
				GetClassName((HWND)wParam, str, MAX_CLASS_NAME);

				if (lstrcmp(g_datZone[i].strClassName, str) != 0)
					continue;

				// Get process id
				GetWindowThreadProcessId((HWND)wParam, &pid);

				// Get process handle
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
				if (hProcess == 0)
					break;

				// Get filename
				GetModuleFileNameEx(hProcess, NULL, str, MAX_PATH);

				// Filtering...
				if (lstrcmp(g_datZone[i].strPath, str) != 0)
					continue;

				// Hook
				if (g_lpPrevMessageProc != NULL)
					break;
				g_hTarget = (HWND)wParam;
				g_hCreatedIndex = i;
				g_lpPrevMessageProc = SetWindowLong(g_hTarget, GWL_WNDPROC, (LONG)hookMessageProc);
			}
		}

		break;
	}
	return CallNextHookEx(g_hHookProc, nCode, wParam, lParam);
}

LRESULT CALLBACK hookMessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	TCHAR	str[255];
	wsprintf(str, "%d\t%x", message, message);
	OutputDebugString(str);

	switch(message)
	{
	case WM_SHOWWINDOW:
		// Maximize
		ShowWindow(hWnd, SW_SHOWMAXIMIZED);

		// Unhook
		SetWindowLong(g_hTarget, GWL_WNDPROC, (LONG)g_lpPrevMessageProc);

		// Initialize
		g_hTarget			= NULL;
		g_hCreatedIndex		= -1;
		g_lpPrevMessageProc = NULL;

		break;
	case WM_GETMINMAXINFO:
		POINT	pt;
		UINT	i;

		if (g_numZone == -1)
			break;

		// Read file for sharing memory
		g_datZone = (DATZONE*)ReadFileMapping(ZONEMAPNAME);
		if (g_datZone == NULL){
			MessageBox(hWnd, "Can't open mapping file!!!", "Error", MB_OK);

			break;
		}

		if (g_hTarget != NULL && g_hCreatedIndex != -1)
		{
			((MINMAXINFO *)lParam)->ptMaxSize.x			= g_datZone[g_hCreatedIndex].crtWnd.right - g_datZone[g_hCreatedIndex].crtWnd.left;
			((MINMAXINFO *)lParam)->ptMaxSize.y			= g_datZone[g_hCreatedIndex].crtWnd.bottom - g_datZone[g_hCreatedIndex].crtWnd.top;
			((MINMAXINFO *)lParam)->ptMaxPosition.x		= g_datZone[g_hCreatedIndex].crtWnd.left;
			((MINMAXINFO *)lParam)->ptMaxPosition.y		= g_datZone[g_hCreatedIndex].crtWnd.top;
			((MINMAXINFO *)lParam)->ptMaxTrackSize.x	= g_datZone[g_hCreatedIndex].crtWnd.right - g_datZone[g_hCreatedIndex].crtWnd.left;
			((MINMAXINFO *)lParam)->ptMaxTrackSize.y	= g_datZone[g_hCreatedIndex].crtWnd.bottom - g_datZone[g_hCreatedIndex].crtWnd.top;

			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		else
		{
			GetCursorPos(&pt);
			for (i=0 ; i<=g_numZone ; i++){
				if (g_datZone[i].crtWnd.left <= pt.x && pt.x <= g_datZone[i].crtWnd.right &&
					g_datZone[i].crtWnd.top	 <= pt.y && pt.y <= g_datZone[i].crtWnd.bottom)
				{
					((MINMAXINFO *)lParam)->ptMaxSize.x			= g_datZone[i].crtWnd.right - g_datZone[i].crtWnd.left;
					((MINMAXINFO *)lParam)->ptMaxSize.y			= g_datZone[i].crtWnd.bottom - g_datZone[i].crtWnd.top;
					((MINMAXINFO *)lParam)->ptMaxPosition.x		= g_datZone[i].crtWnd.left;
					((MINMAXINFO *)lParam)->ptMaxPosition.y		= g_datZone[i].crtWnd.top;
					((MINMAXINFO *)lParam)->ptMaxTrackSize.x	= g_datZone[i].crtWnd.right - g_datZone[i].crtWnd.left;
					((MINMAXINFO *)lParam)->ptMaxTrackSize.y	= g_datZone[i].crtWnd.bottom - g_datZone[i].crtWnd.top;

					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
		}

		break;
	}

	return CallWindowProc((WNDPROC)g_lpPrevMessageProc, hWnd, message, wParam, lParam);
}

LPVOID InitFileMapping(LPCSTR pszShareName, SIZE_T size){
	HANDLE	hFileMap;
	LPVOID	lpMapView;

	// Create file mapping
	hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, pszShareName);
	if(hFileMap == NULL)
		return NULL;

	// Read
	lpMapView = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, size);
	if (lpMapView == NULL)
		return NULL;

	// Release
	//CloseHandle(hFileMap);	->		Error!!!

	return lpMapView;
}

LPVOID ReadFileMapping(LPCSTR pszShareName){
	HANDLE	hFileRead;
	LPVOID	lpMapView;

	// Open file mapping
	hFileRead = OpenFileMapping(FILE_MAP_READ, FALSE, pszShareName);
	if (hFileRead == NULL)
		return NULL;

	lpMapView = MapViewOfFile(hFileRead, FILE_MAP_READ, 0, 0, 0);
	if (lpMapView == NULL)
		return NULL;

	// Release
	CloseHandle(hFileRead);

	return lpMapView;
}

BOOLEAN LoadFormData(LPSTR lpFileName){
	UINT	i;
	TCHAR	stri[3];
	TCHAR	strtmp[MAX_STRING];

	if (!lstrcmp(lpFileName, ""))
		return FALSE;

	g_numZone = GetPrivateProfileInt(TEXT("numZone"), TEXT("numZone"), 0, lpFileName);
	g_datZone = (DATZONE*)InitFileMapping(ZONEMAPNAME, sizeof(DATZONE) * (g_numZone + 1));

	for (i=0 ; i<=g_numZone ; i++)
	{
		_itoa_s(i, (char *)stri, 3, 5);

		// Rectangle
		// GetPrivateProfileInt's return is (-) so use GetPrivateProfileString
		g_datZone[i].crtWnd.left = GetPrivateProfileInt(stri, TEXT("left"), 0, lpFileName);
		g_datZone[i].crtWnd.right = GetPrivateProfileInt(stri, TEXT("right"), 0, lpFileName);
		g_datZone[i].crtWnd.top = GetPrivateProfileInt(stri, TEXT("top"), 0, lpFileName);
		g_datZone[i].crtWnd.bottom = GetPrivateProfileInt(stri, TEXT("bottom"), 0, lpFileName);

		// Class name
		GetPrivateProfileString(stri, TEXT("class name"), 0, strtmp, MAX_STRING, lpFileName);
		lstrcpy(g_datZone[i].strClassName, strtmp);

		// Path
		GetPrivateProfileString(stri, TEXT("path"), 0, strtmp, MAX_STRING, lpFileName);
		lstrcpy(g_datZone[i].strPath, strtmp);

		// Auto resize
		g_datZone[i].isAutoResize = GetPrivateProfileInt(stri, TEXT("auto resize"), 0, lpFileName);
	}

	return TRUE;
}

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) void HookProc(){
		g_hHookProc = SetWindowsHookEx(WH_CBT, hookMinMaxProc, g_hDllInst, 0);

		LoadFormData(g_hFilePath);
	}

	__declspec(dllexport) void UnhookProc(){
		// Release memory
		UnmapViewOfFile(g_datZone);

		// Unhook
		SetWindowLong(g_hTarget, GWL_WNDPROC, g_lpPrevMessageProc);

		if(g_hHookProc == NULL)
			return ;
		UnhookWindowsHookEx(g_hHookProc);
		g_hHookProc = NULL;
	}

	__declspec(dllexport) BOOLEAN LoadData(LPSTR lpFileName){
		lstrcpy(g_hFilePath, lpFileName);

		LoadFormData(g_hFilePath);

		return TRUE;
	}

	__declspec(dllexport) BOOLEAN ReLoadData(){
		LoadFormData(g_hFilePath);

		return TRUE;
	}
#ifdef __cplusplus
}
#endif