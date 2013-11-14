#include "stdafx.h"

extern	HINSTANCE	g_hInst;

HWND*	g_hDlgZone;
UINT	g_numZone		= -1;
BOOL	g_isZoneDrag	= FALSE;
BOOL	g_movHitTest;
LONG	g_oldX, g_oldY;

BOOL CALLBACK dlgZoneProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	POINT	pt;
	RECT	crtDlg, crtCtrl;
	HWND	hGroup;
	HDC		hDC;

	switch(message){
		case WM_INITDIALOG:
			HWND	hChildDlg;
			HWND*	tmpZone;

			//Backup
			if (g_numZone != -1)
			{
				tmpZone = (HWND*)malloc(sizeof(HWND) * (g_numZone + 1));
				memcpy(tmpZone, g_hDlgZone, sizeof(HWND) * (g_numZone + 1));
			}

			g_numZone++;
			g_hDlgZone = (HWND*)malloc(sizeof(HWND) * (g_numZone + 1));
			if (g_numZone != 0)
				memcpy(g_hDlgZone, tmpZone, sizeof(HWND) * (g_numZone + 1));

			// Set global variable
			g_hDlgZone[g_numZone] = hDlg;

			// Show group dialog
			hChildDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_ZONE_GROUP), hDlg, dlgZoneGroupProc);
			ShowWindow(hChildDlg, SW_SHOW);

			return TRUE;
		case WM_DRAWITEM:
			LPDRAWITEMSTRUCT	lpdis;
			HDC					hMemDC;
			HBITMAP				hBitmap;

			// Draw button bitmap
			lpdis = (LPDRAWITEMSTRUCT)lParam;
			switch(lpdis->CtlID)
			{
			case IDC_BUTTON_CLOSE:
				hMemDC = CreateCompatibleDC(lpdis->hDC);
				hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BUTTON_CLOSE));
				SelectObject(hMemDC, hBitmap);
				BitBlt(lpdis->hDC, 0, 0, 31, 17, hMemDC, 0, 0, SRCCOPY);
				break;
			}

			return TRUE;
		case WM_PAINT:
			PAINTSTRUCT	ps;

			// Move controls
			GetClientRect(hDlg, &crtDlg);
			// close button
			MoveWindow(GetDlgItem(hDlg, IDC_BUTTON_CLOSE), crtDlg.right - crtDlg.left - 31 - 5, 5, 31, 17, TRUE);
			// group dialog
			hGroup = FindWindowEx(hDlg, GetDlgItem(hDlg, IDC_BUTTON_CLOSE), TEXT("#32770"), TEXT(""));
			GetClientRect(hGroup, &crtCtrl);
			crtDlg.left		+= ZONE_BORDER_SIZE;
			crtDlg.right	-= ZONE_BORDER_SIZE;
			crtDlg.top		+= ZONE_CAPTION_SIZE;
			crtDlg.bottom	-= ZONE_BORDER_SIZE;
			MoveWindow(hGroup, crtDlg.left + ((crtDlg.right - crtDlg.left) - (crtCtrl.right - crtCtrl.left)) / 2,
				crtDlg.top + ((crtDlg.bottom - crtDlg.top) - (crtCtrl.bottom - crtCtrl.top)) / 2, 
				(crtCtrl.right - crtCtrl.left), (crtCtrl.bottom - crtCtrl.top), TRUE);

			// Draw rectangle to indicate zone
			GetClientRect(hDlg, &crtDlg);
			hDC = BeginPaint(hDlg, &ps);
			Rectangle(hDC, 0, 0, crtDlg.right, crtDlg.bottom);
			Rectangle(hDC, ZONE_BORDER_SIZE - 1, ZONE_CAPTION_SIZE, crtDlg.right - (ZONE_BORDER_SIZE + 1), crtDlg.bottom - (ZONE_BORDER_SIZE + 1));
			EndPaint(hDlg, &ps);

			return TRUE;
		case WM_LBUTTONDOWN:
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			// Set cursor according to NCHITTEST
			g_movHitTest = GetHitTest(hDlg, pt, ZONE_BORDER_SIZE);
			SetResizeCursor(g_movHitTest);

			// Setting for move window
			if(g_movHitTest == HIT_CAPTION){
				g_oldX = pt.x;
				g_oldY = pt.y;
			}

			// Set capture for outing control
			SetCapture(hDlg);

			// Drag setting
			g_isZoneDrag = TRUE;

			return TRUE;
		case WM_MOUSEMOVE:
			UINT	posHit;

			if(g_isZoneDrag == FALSE){
				// "pt.x = LOWORD(lParam)" -> Error becuase (-)
				// "pt.y = HIWORD(lParam)" -> Error becuase (-)
				GetCursorPos(&pt);
				GetWindowRect(hDlg, &crtDlg);
				pt.x -= crtDlg.left;
				pt.y -= crtDlg.top;

				// Set cursor according to NCHITTEST
				posHit = GetHitTest(hDlg, pt, ZONE_BORDER_SIZE);
				SetResizeCursor(posHit);
			}
			else{
				GetWindowRect(hDlg, &crtDlg);

				// Get cursor position
				GetCursorPos(&pt);

				// Resize window and move
				switch(g_movHitTest)
				{
				case HIT_TOP:
					if(crtDlg.bottom - pt.y < MIN_HEIGHT)
						break;
					MoveWindow(hDlg, crtDlg.left, pt.y, crtDlg.right - crtDlg.left, crtDlg.bottom - pt.y, TRUE);
					break;
				case HIT_BOTTOM:
					if(pt.y - crtDlg.top < MIN_HEIGHT)
						break;
					MoveWindow(hDlg, crtDlg.left, crtDlg.top, crtDlg.right - crtDlg.left, pt.y - crtDlg.top, TRUE);
					break;
				case HIT_LEFT:
					if(crtDlg.right - pt.x < MIN_WIDTH)
						break;
					MoveWindow(hDlg, pt.x, crtDlg.top, crtDlg.right - pt.x, crtDlg.bottom - crtDlg.top, TRUE);
					break;
				case HIT_RIGHT:
					if(pt.x - crtDlg.left < MIN_WIDTH)
						break;
					MoveWindow(hDlg, crtDlg.left, crtDlg.top, pt.x - crtDlg.left, crtDlg.bottom - crtDlg.top, TRUE);
					break;
				case HIT_TOP | HIT_LEFT:
					if(crtDlg.right - pt.x < MIN_WIDTH && crtDlg.bottom - pt.y < MIN_HEIGHT)
						break;
					else if(crtDlg.right - pt.x < MIN_WIDTH)
						pt.x = crtDlg.left;
					else if(crtDlg.bottom - pt.y < MIN_HEIGHT)
						pt.y = crtDlg.top;
					MoveWindow(hDlg, pt.x, pt.y, crtDlg.right - pt.x, crtDlg.bottom - pt.y, TRUE);
					break;
				case HIT_BOTTOM | HIT_RIGHT:
					if(pt.x - crtDlg.left < MIN_WIDTH && pt.y - crtDlg.top < MIN_HEIGHT)
						break;
					else if(pt.x - crtDlg.left < MIN_WIDTH)
						pt.x = crtDlg.right;
					else if(pt.y - crtDlg.top < MIN_HEIGHT)
						pt.y = crtDlg.bottom;
					MoveWindow(hDlg, crtDlg.left, crtDlg.top, pt.x - crtDlg.left, pt.y - crtDlg.top, TRUE);
					break;
				case HIT_TOP | HIT_RIGHT:
					if(pt.x - crtDlg.left < MIN_WIDTH && crtDlg.bottom - pt.y < MIN_HEIGHT)
						break;
					else if(pt.x - crtDlg.left < MIN_WIDTH)
						pt.x = crtDlg.right;
					else if(crtDlg.bottom - pt.y < MIN_HEIGHT)
						pt.y = crtDlg.top;
					MoveWindow(hDlg, crtDlg.left, pt.y, pt.x - crtDlg.left, crtDlg.bottom - pt.y, TRUE);
					break;
				case HIT_BOTTOM | HIT_LEFT:
					if(crtDlg.right - pt.x < MIN_WIDTH && pt.y - crtDlg.top < MIN_HEIGHT)
						break;
					else if(crtDlg.right - pt.x < MIN_WIDTH)
						pt.x = crtDlg.left;
					else if(pt.y - crtDlg.top < MIN_HEIGHT)
						pt.y = crtDlg.bottom;
					MoveWindow(hDlg, pt.x, crtDlg.top, crtDlg.right - pt.x, pt.y - crtDlg.top, TRUE);
					break;
				case HIT_CAPTION:
					MoveWindow(hDlg, -(g_oldX - pt.x), -(g_oldY - pt.y), crtDlg.right - crtDlg.left, crtDlg.bottom - crtDlg.top, TRUE);
					break;
				}

				// Redraw
				InvalidateRect(hDlg, NULL, TRUE);
			}

			return TRUE;
		case WM_LBUTTONUP:
			// Drag...?
			if(g_isZoneDrag == FALSE)
				return TRUE;

			// Release capture for outing control
			ReleaseCapture();

			// Initialize drag settings
			g_isZoneDrag = FALSE;

			// Redraw
			InvalidateRect(hDlg, NULL, TRUE);

			return TRUE;
		case WM_MOVE:
			hGroup = FindWindowEx(hDlg, GetDlgItem(hDlg, IDC_BUTTON_CLOSE), TEXT("#32770"), TEXT(""));

			GetWindowRect(hDlg, &crtDlg);
			SetDlgItemInt(hGroup, IDC_EDIT_X, crtDlg.left, TRUE);
			SetDlgItemInt(hGroup, IDC_EDIT_Y, crtDlg.top, TRUE);

			return TRUE;

		case WM_SIZE:
			hGroup = FindWindowEx(hDlg, GetDlgItem(hDlg, IDC_BUTTON_CLOSE), TEXT("#32770"), TEXT(""));

			GetWindowRect(hDlg, &crtDlg);
			SetDlgItemInt(hGroup, IDC_EDIT_WIDTH, crtDlg.right - crtDlg.left, TRUE);
			SetDlgItemInt(hGroup, IDC_EDIT_HEIGHT, crtDlg.bottom - crtDlg.top, TRUE);

			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_CLOSE:
				UINT	i;
				UINT	tmpZone;

				// Remove zone
				tmpZone = g_numZone;
				g_numZone -= 1;
				for (i=0 ; i<=tmpZone ; i++)
					if (g_hDlgZone[i] == hDlg)
						break;
				for (; i<=tmpZone ; i++)
					g_hDlgZone[i] = g_hDlgZone[i+1];

				// Reload subject list
				LoadSubjectList();

				// End dialog
				EndDialog(hDlg, IDC_BUTTON_CLOSE);
				break;
			}

			return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK dlgZoneGroupProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message)
	{
	case WM_INITDIALOG:
		TCHAR	str[MAX_STRING];

		wsprintf(str, TEXT("Zone%d"), g_numZone + 1);
		SetDlgItemText(hDlg, IDC_EDIT_SUBJECT, str);

		// Re list finder's subjects
		LoadSubjectList();

		return TRUE;
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case EN_CHANGE:
			switch(LOWORD(wParam))
			{
			case IDC_EDIT_SUBJECT:
				LoadSubjectList();

				break;
			}

			break;
		case EN_KILLFOCUS:
			RECT	crtWnd;

			GetWindowRect(GetParent(hDlg), &crtWnd);
			switch(LOWORD(wParam))
			{
			case IDC_EDIT_X:
				MoveWindow(GetParent(hDlg), GetDlgItemInt(hDlg, IDC_EDIT_X, NULL, FALSE), crtWnd.top,
					crtWnd.right - crtWnd.left, crtWnd.bottom - crtWnd.top, TRUE);

				break;
			case IDC_EDIT_Y:
				MoveWindow(GetParent(hDlg), crtWnd.left, GetDlgItemInt(hDlg, IDC_EDIT_Y, NULL, FALSE),
					crtWnd.right - crtWnd.left, crtWnd.bottom - crtWnd.top, TRUE);

				break;
			case IDC_EDIT_WIDTH:
				MoveWindow(GetParent(hDlg), crtWnd.left, crtWnd.top,
					GetDlgItemInt(hDlg, IDC_EDIT_WIDTH, NULL, FALSE), crtWnd.bottom - crtWnd.top, TRUE);

				break;
			case IDC_EDIT_HEIGHT:
				MoveWindow(GetParent(hDlg), crtWnd.left, crtWnd.top,
					crtWnd.right - crtWnd.left, GetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, NULL, FALSE), TRUE);

				break;
			}

			// Redraw
			InvalidateRect(GetParent(hDlg), NULL, TRUE);

			break;
		}
	}
	return FALSE;
}

UINT GetHitTest(HWND hWnd, POINT pt, UINT border){
	UINT	posHit;
	RECT	crtWnd;

	// Get mouse position
	posHit = 0;
	GetWindowRect(hWnd, &crtWnd);
	if(pt.x < ZONE_BORDER_SIZE)
		posHit += HIT_LEFT;
	else if(crtWnd.right - crtWnd.left - ZONE_BORDER_SIZE - (ZONE_BORDER_SIZE / 3) < pt.x)
		posHit += HIT_RIGHT;
	if(pt.y < ZONE_BORDER_SIZE / 3)
		posHit += HIT_TOP;
	else if(crtWnd.bottom - crtWnd.top - ZONE_BORDER_SIZE - (ZONE_BORDER_SIZE / 3) < pt.y)
		posHit += HIT_BOTTOM;
	if(ZONE_BORDER_SIZE <= pt.x && pt.x <= crtWnd.right - crtWnd.left - ZONE_BORDER_SIZE - (ZONE_BORDER_SIZE / 3) &&
		ZONE_BORDER_SIZE / 3 <= pt.y && pt.y <= ZONE_CAPTION_SIZE)
		posHit += HIT_CAPTION;

	return posHit;
}

BOOLEAN SetResizeCursor(UINT posHit){
	// Set cursor for resizing
	switch(posHit)
	{
	case HIT_TOP:
	case HIT_BOTTOM:
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
		break;
	case HIT_LEFT:
	case HIT_RIGHT:
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		break;
	case HIT_TOP | HIT_LEFT:
	case HIT_BOTTOM | HIT_RIGHT:
		SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		break;
	case HIT_TOP | HIT_RIGHT:
	case HIT_BOTTOM | HIT_LEFT:
		SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		break;
	}

	return TRUE;
}