// win32painter.h
// Aswin van Woudenberg

#ifndef WIN32PAINTER_H
#define WIN32PAINTER_H

#include <windows.h>
#include "generic/PPlot.h"

class Win32Painter: public Painter 
{
public:
	PPlot pPlot;

	/**
	 * The constructor
	 * @param hWndParent The HWND of the parent
	 * @param id The id of the control
	 */
	Win32Painter(HWND hWndParent, int id) 
		: _hWnd(GetDlgItem(hWndParent, id)) { }

	/**
	 * Draw the plot
	 * @param lpDrawItem A pointer to a DRAWITEMSTRUCT
	 */
	void OnDrawItem(LPDRAWITEMSTRUCT lpDrawItem)
	{
		_hDC = lpDrawItem->hDC;
		_hPen = NULL;
		_hBrush = NULL;
		// Draw background
		SetFillColor(255, 255, 255);
		FillRect(0, 0, GetWidth(), GetHeight());

		::SetBkMode(_hDC, TRANSPARENT);
		pPlot.Draw(*this);
		
		// Possible clean up selected pen
		if (_hPen != NULL)
			::DeleteObject(_hPen);
		
		// Clean up selected brush
		if (_hBrush != NULL)
			::DeleteObject(_hBrush);
	}

	void DrawLine(float inX1, float inY1, float inX2, float inY2) 
	{ 
		::MoveToEx(_hDC, (int)inX1, (int)inY1, NULL);
		::LineTo(_hDC, (int)inX2, (int)inY2);
	}

	void FillRect(int inX, int inY, int inW, int inH) 
	{
		RECT rect = {0};
		rect.left = inX;
		rect.top = inY;
		rect.right = inW + inX;
		rect.bottom = inH + inY;
		::FillRect(_hDC, &rect, _hBrush);
	}
	
	void InvertRect(int inX, int inY, int inW, int inH) 
	{ 
		int modeOld = ::GetROP2(_hDC);
		::SetROP2(_hDC, R2_NOT);
		::Rectangle(_hDC, inX, inY, inW + inX, inH + inY);
		::SetROP2(_hDC, modeOld);
	}

	void SetClipRect(int inX, int inY, int inW, int inH) 
	{ 
		HRGN clip = ::CreateRectRgn(inX, inY, inW + inX, inH + inY);
		::SelectClipRgn(_hDC, clip);
	}
	
	long GetWidth() const 
	{
		RECT rect = {0};
		::GetWindowRect(_hWnd, &rect);
		return rect.right - rect.left;
	}

	long GetHeight() const 
	{ 
		RECT rect = {0};
		::GetWindowRect(_hWnd, &rect);
		return rect.bottom - rect.top; 
	}

	void SetLineColor(int inR, int inG, int inB) 
	{ 
		if (_hPen != NULL)
			::DeleteObject(_hPen);
		_hPen = ::CreatePen(PS_SOLID, 1, RGB(inR, inG, inB));
		::SelectObject(_hDC, _hPen);
		::SetTextColor(_hDC, RGB(inR, inG, inB));
	}

	void SetFillColor(int inR, int inG, int inB) 
	{
		if (_hBrush != NULL)
			::DeleteObject(_hBrush);
		_hBrush = ::CreateSolidBrush(RGB(inR, inG, inB));
		::SelectObject(_hDC, _hBrush);
	}

	long CalculateTextDrawSize(const char *inString) 
	{ 
		SIZE size = {0};
		::GetTextExtentPoint32A(_hDC, inString, strlen(inString), &size);
		return size.cx; 
	}
	
	long GetFontHeight() const 
	{ 
		SIZE size = {0};
		::GetTextExtentPoint32A(_hDC, "gI", 2, &size);
		return size.cy; 
	}
	
	void DrawText(int inX, int inY, const char *inString) 
	{ 
		::TextOutA(_hDC, inX, inY - GetFontHeight(), inString, strlen(inString));
	}

	void DrawRotatedText(int inX, int inY, float inDegrees2, const char *inString) 
	{
		bool bEnableAngleChar = true;
		int nFontSize = 9;
		float inDegrees = inDegrees2 * -1;

		LOGFONT lf = {0};
		_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
		lf.lfWeight = FW_NORMAL;

		// Set the background mode to transparent for the
		// text-output operation.
		int nOldBkMode = SetBkMode(_hDC, TRANSPARENT);
		// Specify the angle to draw line
		lf.lfEscapement = (LONG)(inDegrees*10);

		int nOldGMode;
		if (bEnableAngleChar) // Enable character angle
		{
			// Set graphics mode to advance to enable orientation
			nOldGMode = ::SetGraphicsMode(_hDC, GM_ADVANCED );
			// Specify the angle of characters
			lf.lfOrientation = (LONG)(inDegrees*10);
		}
		else // Draw in normal mode
		{
			nOldGMode = ::SetGraphicsMode(_hDC, GM_COMPATIBLE );
		}

		lf.lfHeight = -MulDiv(nFontSize, ::GetDeviceCaps(_hDC, LOGPIXELSY), 72);

		// Select the new font created
		HFONT hFont = reinterpret_cast<HFONT>(::CreateFontIndirect(&lf));
		HFONT hPrevFont = reinterpret_cast<HFONT>(::SelectObject(_hDC, hFont));

		// Draw text to screen
		::TextOutA(_hDC, inX - GetFontHeight(), inY, inString, strlen(inString));
		::SelectObject(_hDC, hPrevFont);
		::DeleteObject(hFont);

		// Restore old values
		::SetBkMode(_hDC, nOldBkMode);
		::SetGraphicsMode(_hDC, nOldGMode);
	}

	/**
	 * For casting to the window's handle of the component
	 * @return The components window's handle
	 */
	operator HWND() const { return _hWnd; }

private:
	HWND _hWnd;
	HDC _hDC;
	HPEN _hPen;
	HBRUSH _hBrush;
};

#endif // WIN32PAINTER_H
