#include <time.h>
#include "T9.h"
#include "Keyboard.h"

const char* t9keys[] = { " ,.0?!'\\`\":;", "@*1/-+=#$%^&~|(){}[]", "abc2ABC", "def3DEF", "ghi4GHI", "jkl5JKL", "mno6MNO", "pqrs7PRQS", "tuv8TUV", "wxyz9WXYZ" };

char buffer[1024] = { 0 };

time_t lastPressTime = 0;
int keyIndex = 0;
int lastKey = 0;


HWND windowHandle = 0;
LPCWSTR text = L"";
unsigned long lastTS = 0;
bool isShowing = false;

RECT dest;
HDC hDC;
HGDIOBJ font;
HFONT hFontOld;

void t9(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
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
}

void t9send(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	SendKeys(buffer);
	clear();
}

void t9back(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	if (strlen(buffer) > 0)
	{
		buffer[strlen(buffer) - 1] = 0; //Zero out the last character to backspace
	}
	else
	{
		SendVK(VK_BACK);
	}
	drawText(buffer);
}

void t9clear(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	clear();
}

void clear()
{
	memset(buffer, 0, sizeof(buffer));
	drawText(buffer);
}

void initializeT9Form()
{
	std::string junk;
	dest.top = GetSystemMetrics(SM_CYSCREEN) - 800;
	dest.left = 0;
	dest.bottom = GetSystemMetrics(SM_CYSCREEN);
	dest.right = GetSystemMetrics(SM_CXSCREEN);
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
				GetSystemMetrics(SM_CXSCREEN),
				GetSystemMetrics(SM_CYSCREEN),
				NULL,
				NULL,
				NULL,
				NULL);
			SetWindowLong(windowHandle, GWL_EXSTYLE, GetWindowLong(windowHandle, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW);
			SetLayeredWindowAttributes(windowHandle,
				RGB(255, 255, 255),
				255,
				LWA_COLORKEY);

			hDC = GetWindowDC(windowHandle);
			SetTextColor(hDC, RGB(50, 230, 70));
			font = CreateFont(512, 0, 0, 0, 1000, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial");
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

void drawText(const char* _text)
{
	if (strlen(_text) == 0)
	{
		text = L"";
		InvalidateRect(windowHandle, NULL, TRUE);
		ShowWindow(windowHandle, SW_HIDE);
	}
	else
	{
		text = StringToLPCWSTR(_text);
		InvalidateRect(windowHandle, NULL, TRUE);
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
	case WM_CHAR: //this is just for a program exit besides window's borders/task bar
		if (wparam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		if (wparam == 1) //1 for hide
		{
			if (lastTS + 4500 < GetTickCount64())
			{
				ShowWindow(windowHandle, SW_HIDE);
			}
		}
		else if (wparam == 2) //2 for show
		{
			lastTS = GetTickCount64();
			ShowWindow(windowHandle, SW_MAXIMIZE);
			DrawText(hDC, text, wcslen(text), &dest, DT_SINGLELINE | DT_NOCLIP);
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