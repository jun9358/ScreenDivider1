#include <CommDlg.h>

#define ID_TOOLBAR						10
#define IDC_TOOLBAR_SEP					0
#define IDC_TOOLBAR_NEW					11
#define IDC_TOOLBAR_OPEN				12
#define IDC_TOOLBAR_SAVE				13
#define IDC_TOOLBAR_SAVEAS				14
#define IDC_TOOLBAR_FINDER				15
#define IDC_TOOLBAR_TRANSPARENT			16
#define IDC_TOOLBAR_EXIT				17

BOOL CALLBACK dlgToolbarProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOLEAN	NewForm(void);
BOOLEAN	SaveFormFile(LPSTR	lpFileName);
BOOLEAN	LoadFormFile(LPSTR	lpFileName);