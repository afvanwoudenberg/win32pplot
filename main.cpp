// main.cpp
// Aswin van Woudenberg

#include <windows.h>
#include <tchar.h>

#include "resource.h"
#include "generic/PPlot.h"
#include "win32painter.h"

BOOL CALLBACK PPlotDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

Win32Painter *pplot1, *pplot2, *pplot3, *pplot4, *pplot5, *pplot6, *pplot7, *pplot8;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow)
{
	return ::DialogBox(hInstance, MAKEINTRESOURCE(IDD_WIN32PPLOT), NULL, PPlotDlgProc);
}

BOOL CALLBACK PPlotDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_INITDIALOG:
			pplot1 = new Win32Painter(hwnd, IDC_PPLOT1);
			pplot2 = new Win32Painter(hwnd, IDC_PPLOT2);
			pplot3 = new Win32Painter(hwnd, IDC_PPLOT3);
			pplot4 = new Win32Painter(hwnd, IDC_PPLOT4);
			pplot5 = new Win32Painter(hwnd, IDC_PPLOT5);
			pplot6 = new Win32Painter(hwnd, IDC_PPLOT6);
			pplot7 = new Win32Painter(hwnd, IDC_PPLOT7);
			pplot8 = new Win32Painter(hwnd, IDC_PPLOT8);
			MakeExamplePlot1(pplot1->pPlot);
			MakeExamplePlot2(pplot2->pPlot);
			MakeExamplePlot3(pplot3->pPlot);
			MakeExamplePlot4(pplot4->pPlot);
			MakeExamplePlot5(pplot5->pPlot);
			MakeExamplePlot6(pplot6->pPlot);
			MakeExamplePlot7(pplot7->pPlot);
			MakeExamplePlot8(pplot8->pPlot);
			return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDCANCEL:
					delete pplot1;
					delete pplot2;
					delete pplot3;
					delete pplot4;
					delete pplot5;
					delete pplot6;
					delete pplot7;
					delete pplot8;
					EndDialog(hwnd, 0);
					break;
			}
			break;
		case WM_DRAWITEM:
			switch (wParam)
			{
				case IDC_PPLOT1:
					pplot1->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
					break;
				case IDC_PPLOT2:
					pplot2->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
					break;
				case IDC_PPLOT3:
					pplot3->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
					break;
				case IDC_PPLOT4:
					pplot4->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
					break;
				case IDC_PPLOT5:
					pplot5->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
					break;
				case IDC_PPLOT6:
					pplot6->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
					break;
				case IDC_PPLOT7:
					pplot7->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
					break;
				case IDC_PPLOT8:
					pplot8->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
					break;
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}
