#include "stdafx.h"

extern	HWND	g_hDlgBoard;

HWND	g_hDlgTransparent;

BOOL CALLBACK dlgTransparentProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	HWND	hSlide;

	switch(message)
	{
	case WM_INITDIALOG:
		// Set handle
		g_hDlgTransparent = hDlg;

		// Set range and init value
		hSlide = GetDlgItem(hDlg, IDC_SLIDER_TRANSPARENT);
		SendMessage(hSlide, TBM_SETRANGE, FALSE, MAKELPARAM(1, 255));
		SendMessage(hSlide, TBM_SETPOS, TRUE, 100);

		return TRUE;

	case WM_HSCROLL:
		UINT	pos;

		hSlide = GetDlgItem(hDlg, IDC_SLIDER_TRANSPARENT);
		if ((HWND)lParam == hSlide)
		{
			// Set transparent value
			pos = SendMessage(hSlide, TBM_GETPOS, 0, 0);
			SetLayeredWindowAttributes(g_hDlgBoard, 0, pos, LWA_ALPHA);
		}

		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			ShowWindow(hDlg, SW_HIDE);

			break;
		}

		return TRUE;
	}
	return FALSE;
}