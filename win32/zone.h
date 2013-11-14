#define ZONE_BORDER_SIZE		9
#define ZONE_CAPTION_SIZE		26

//////////////////////
//	¦£¦¡¦¨¦¡¦¨¦¡¦¤	//
//	¦¢03¦¢01¦¢05¦¢	//
//	¦§¦¡¦«¦¡¦«¦¡¦©	//
//	¦¢02¦¢00¦¢04¦¢	//
//	¦§¦¡¦«¦¡¦«¦¡¦©	//
//	¦¢10¦¢08¦¢12¦¢	//
//	¦¦¦¡¦ª¦¡¦ª¦¡¦¥	//
//////////////////////
#define HIT_TOP		1
#define HIT_LEFT	2
#define HIT_RIGHT	4
#define HIT_BOTTOM	8
#define HIT_CAPTION 16

BOOL CALLBACK dlgZoneProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dlgZoneGroupProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
UINT GetHitTest(HWND hWnd, POINT pt, UINT border);
BOOLEAN SetResizeCursor(UINT posHit);