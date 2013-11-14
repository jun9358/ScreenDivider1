#pragma comment(lib, "psapi.lib")
#include <psapi.h>

#define MAX_STRING	255
#define ZONEMAPNAME		"MAPPINGDATZONE"
#define SETTINGMAPNAME	"MAPPINGSETTING"

typedef struct DATZONE
{
	//TCHAR	strSubject[MAX_STRING];
	RECT	crtWnd;
	TCHAR	strClassName[MAX_CLASS_NAME];
	TCHAR	strPath[MAX_PATH];
	//BOOLEAN	isAutoRun;
	BOOLEAN	isAutoResize;
	//BOOLEAN	isMagnetic;
	//SHORTCUT	shortcut;
}datZone;

LRESULT CALLBACK hookMinMaxProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK hookMessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LPVOID InitFileMapping(LPCSTR pszShareName, SIZE_T size);
LPVOID ReadFileMapping(LPCSTR pszShareName);
BOOLEAN LoadFormData(LPSTR lpFileName);