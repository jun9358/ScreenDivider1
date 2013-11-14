#include "stdafx.h"

HHOOK	g_hHookProc;
LONG	g_lpPrevMessageProc;

BOOLEAN	HookProc(void){
	HMODULE			hDll;
	PFN_HOOKPROC	pfnHookProc;

	// Initializing...
	hDll			= NULL;
	pfnHookProc		= NULL;

	// Load dll
	hDll = LoadLibrary(DEF_DLL_NAME);
	if (!hDll)
		return FALSE;

	// Get function's address
	pfnHookProc = (PFN_HOOKPROC)GetProcAddress(hDll, DEF_HOOKPROC);
	if (!pfnHookProc)
		return FALSE;

	pfnHookProc();

	// Free dll
	FreeLibrary(hDll);

	return TRUE;
}

BOOLEAN	UnhookProc(void){
	HMODULE			hDll;
	PFN_UNHOOKPROC	pfnUnhookProc;

	// Initializing...
	hDll			= NULL;
	pfnUnhookProc	= NULL;

	// Load dll
	hDll = LoadLibrary(DEF_DLL_NAME);
	if (!hDll)
		return FALSE;

	// Get function's address
	pfnUnhookProc = (PFN_UNHOOKPROC)GetProcAddress(hDll, DEF_UNHOOKPROC);
	if (!pfnUnhookProc)
		return FALSE;

	pfnUnhookProc();

	// Free dll
	FreeLibrary(hDll);

	return TRUE;
}

BOOLEAN	LoadDataForDll(LPSTR lpFileName){
	HMODULE			hDll;
	PFN_LOADDATA	pfnLoadData;

	// Initializing...
	hDll			= NULL;
	pfnLoadData		= NULL;

	// Load dll
	hDll = LoadLibrary(DEF_DLL_NAME);

	// Get function's address
	pfnLoadData	= (PFN_LOADDATA)GetProcAddress(hDll, DEF_LOADDATA);

	pfnLoadData(lpFileName);

	// Free dll
	FreeLibrary(hDll);

	return TRUE;
}

BOOLEAN ReLoadDataForDll(void){
	HMODULE			hDll;
	PFN_RELOADDATA	pfnReLoadData;

	// Initializing...
	hDll			= NULL;
	pfnReLoadData	= NULL;

	// Load dll
	hDll = LoadLibrary(DEF_DLL_NAME);

	// Get function's address
	pfnReLoadData	= (PFN_RELOADDATA)GetProcAddress(hDll, DEF_LOADDATA);

	pfnReLoadData();

	// Free dll
	FreeLibrary(hDll);

	return TRUE;
}