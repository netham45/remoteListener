#include <time.h>
#include "T9.h"
#include "Keyboard.h"
#include <strsafe.h>

const char* t9keys[] = { " ,.0?!'\\`\":;", "@*1/-+=#$%^&~|(){}[]", "abc2ABC", "def3DEF", "ghi4GHI", "jkl5JKL", "mno6MNO", "pqrs7PRQS", "tuv8TUV", "wxyz9WXYZ" };

char buffer[1024] = { 0 };

time_t lastPressTime = 0;
int keyIndex = 0;
int lastKey = -1;


HWND windowHandle = 0;
LPCWSTR text = L"";
LPCWSTR hintText = L"";
int hintTextIndex = 0;
ULONGLONG lastTS = 0;
bool isShowing = false;

RECT dest;
RECT hintDest;
HDC hDC;
HGDIOBJ font;
HGDIOBJ hintFont;
HFONT hFontOld;

void t9(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	int key = atoi(keycode);
	if (numRepeat > 0 && numRepeat % 5 != 0) //Don't send keys as fast as the remote sends input
		return;
	time_t currentTime = time(NULL);
	double seconds = difftime(currentTime, lastPressTime); //Get the time since the last keypress
	lastPressTime = currentTime;
	if (seconds < 2 && strlen(buffer) > 0 && lastKey == key)
	{
		++keyIndex %= strlen(t9keys[key]); //Cycle through characters in the string
		buffer[strlen(buffer) - 1] = t9keys[key][keyIndex]; //And set the last character of the buffer to the string
	}
	else
	{
		keyIndex = 0;
		buffer[strlen(buffer)] = t9keys[key][0]; //Add to the end of the buffer
	}
	lastKey = key;
	drawText(buffer);
	drawHintText(t9keys[key], keyIndex);
}

void t9send(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (strlen(buffer) > 0)
	{
		SendKeys(buffer);
		clear();
	}
	else
	{
		SendKey(VK_RETURN);
	}
}

void t9back(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (strlen(buffer) > 0)
	{
		buffer[strlen(buffer) - 1] = 0; //Zero out the last character to backspace
		lastPressTime = time(NULL);
	}
	else
	{
		SendVK(VK_BACK);
	}
	lastKey = -1;
	drawHintText("", 0);
	drawText(buffer);
}

void t9clear(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	clear();
}

void clear()
{
	memset(buffer, 0, sizeof(buffer));
	drawText(buffer);
	drawHintText(buffer,0);
}

void initializeT9Form()
{
	std::string junk;
	dest.top = GetSystemMetrics(SM_CYSCREEN) - 500;
	dest.left = 0;
	dest.bottom = GetSystemMetrics(SM_CYSCREEN);
	dest.right = GetSystemMetrics(SM_CXSCREEN);
	hintDest.top = GetSystemMetrics(SM_CYSCREEN) - 200;
	hintDest.left = 0;
	hintDest.bottom = GetSystemMetrics(SM_CYSCREEN);
	hintDest.right = GetSystemMetrics(SM_CXSCREEN);
	auto t = CreateThread(nullptr, 0, [](void*) -> DWORD
		{
			WNDCLASS windowClass = { 0 };
			windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hInstance = NULL;
			windowClass.lpfnWndProc = WndProc;
			windowClass.lpszClassName = L"RemoteListener_T9";
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			if (!RegisterClass(&windowClass))
				MessageBox(NULL, L"Could not register class", L"Error", MB_OK);
			windowHandle = CreateWindow(L"RemoteListener_T9",
				NULL,
				WS_POPUP,
				0,
				0,
				GetSystemMetrics(SM_CXFULLSCREEN),
				GetSystemMetrics(SM_CYFULLSCREEN),
				NULL,
				NULL,
				NULL,
				NULL);
			SetWindowPos(windowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			SetWindowLong(windowHandle, GWL_EXSTYLE, GetWindowLong(windowHandle, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
			SetLayeredWindowAttributes(windowHandle,
				RGB(255, 255, 255),
				255,
				LWA_COLORKEY);

			hDC = GetWindowDC(windowHandle);
			SetTextColor(hDC, RGB(50, 230, 70));
			font = CreateFont(256, 0, 0, 0, 512, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, L"Verdana");
			hintFont = CreateFont(128, 0, 0, 0, 512, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, L"Verdana");
			hFontOld = (HFONT)SelectObject(hDC, font);

			MSG messages;
			while (GetMessage(&messages, NULL, 0, 0) > 0)
			{
				TranslateMessage(&messages);
				DispatchMessage(&messages);
				Sleep(100);
			}
			return 0;
		},
		nullptr, 0, nullptr);
}

void drawHintText(const char* _text, int highlightIndex)
{
	if (strlen(_text) == 0)
	{
		hintText = L"";
	}
	else
	{
		hintTextIndex = highlightIndex;
		hintText = StringToLPCWSTR(_text);
		HDC hdc = GetDC(windowHandle);
		PostMessage(windowHandle, WM_PAINT, 2, 0);
	}
}

void drawText(const char* _text)
{
	if (strlen(_text) == 0)
	{
		text = L"";
		InvalidateRect(windowHandle, NULL, TRUE);
		ShowWindow(windowHandle, SW_HIDE);
		isShowing = false;
	}
	else
	{
		text = StringToLPCWSTR(_text);
		HDC hdc = GetDC(windowHandle);
		PostMessage(windowHandle, WM_PAINT, 2, 0);
	}
}

LPCWSTR StringToLPCWSTR(const char* s)
{

	size_t size = strlen(s) + 1;
	wchar_t* widestr = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, widestr, size, s, size-1);
	return widestr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		if (wparam == 1) //1 for hide
		{
			time_t currentTime = time(NULL);
			double seconds = difftime(currentTime, lastPressTime); //Get the time since the last keypress
			if (seconds > 4.5)
			{
				isShowing = false;
				ShowWindow(windowHandle, SW_HIDE);
			}
		}
		else if (wparam == 2) //2 for show
		{
			time_t currentTime = time(NULL);
			double seconds = difftime(currentTime, lastPressTime); //Get the time since the last keypress
			if (!isShowing && seconds < 2)
			{
				ShowWindow(windowHandle, SW_NORMAL);
				SetWindowPos(windowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				SetWindowLong(windowHandle, GWL_EXSTYLE, GetWindowLong(windowHandle, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
				isShowing = true;
			}
			Rectangle(hDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			if (seconds < 2)
			{
				SetTextColor(hDC, RGB(50, 230, 70));
				SelectObject(hDC, hintFont);
				hintDest.left = 0;
				DrawText(hDC, hintText, (int)wcslen(hintText), &hintDest, 0);
				SetTextColor(hDC, RGB(200, 60, 35));
				SIZE sz;
				GetTextExtentPoint32(hDC, hintText, hintTextIndex, &sz);
				hintDest.left = sz.cx;
				DrawText(hDC, &hintText[hintTextIndex], 1, &hintDest, 0);
			}
			SelectObject(hDC, font);
			SetTextColor(hDC, RGB(50, 230, 70));
			DrawText(hDC, text, (int)wcslen(text), &dest, 0);
			CreateThread(nullptr, 0, [](void*) -> DWORD
				{
					Sleep(2000);
					PostMessage(windowHandle, WM_PAINT, 2, 0);
					return 0;
				},
				nullptr, 0, nullptr);
			CreateThread(nullptr, 0, [](void*) -> DWORD
				{
					Sleep(5000);
					PostMessage(windowHandle, WM_PAINT, 1, 0);
					return 0;
				},
				nullptr, 0, nullptr);
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}

void registerT9Actions()
{
	registerActionCallback(&t9, "t9");
	registerActionCallback(&t9send, "t9Send");
	registerActionCallback(&t9back, "t9Back");
	initializeT9Form();
}