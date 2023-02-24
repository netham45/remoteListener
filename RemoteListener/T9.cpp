#include <time.h>
#include "T9.h"
#include "Keyboard.h"
#include <strsafe.h>
#include "wordlist.h"

const char* phoneKeys[] = { " ,.0?!'\\`\":;", "@*1/-+=#$%^&~|(){}[]", "abc2ABC", "def3DEF", "ghi4GHI", "jkl5JKL", "mno6MNO", "pqrs7PRQS", "tuv8TUV", "wxyz9WXYZ" };

char textBuffer[1024] = { 0 };

char phoneKeyBuffer[1024];

time_t lastPressTime = 0;
time_t lastPhoneLetterTime = 0;
int keyIndex = 0;
int lastKey = -1;


HWND windowHandle = 0;
LPCWSTR text = L"";
LPCWSTR hintText = L"";
std::string hintTextStr = "";
int hintTextIndex1 = 0;
int hintTextLength1 = 0;
int hintTextIndex2 = 0;
int hintTextLength2 = 0;
int textIndex1 = 0;
int textLength1 = 0;
int t9WordIndex = 0;
ULONGLONG lastTS = 0;
bool isShowing = false;

RECT dest;
RECT hintDest;
HDC hDC;
HGDIOBJ font;
HGDIOBJ hintFont;
HFONT hFontOld;
#define T9_MAX_NUMBERS 32
char t9Numbers[T9_MAX_NUMBERS];

int t9getNumbersLen()
{
	for (int i = 0; i < T9_MAX_NUMBERS; i++)
	{
		if (t9Numbers[i] == 0)
			return i;
	}
	return -1;
}

void findT9Word()
{
	char buffer[T9_MAX_NUMBERS];
	for (int i = 0; i < T9_MAX_NUMBERS; i++)
	{
		if (t9Numbers[i] != 0)
		{
			sprintf_s(buffer+i, 2, "%i", t9Numbers[i]);
		}
		else
		{
			buffer[i] = 0;
			break;
		}
	}
	Node *wordNode = search(buffer, rootWordNode);
	hintTextStr = "";
	if (lastKey != -1)
		hintTextStr = phoneKeys[lastKey];
	int i = 0;
	Node* selectedNode = 0;
	Node *phoneKeyNode = new Node();
	phoneKeyNode->word = phoneKeyBuffer;
	phoneKeyNode->next = 0;
	phoneKeyNode->children = {0};

	Node* Children[NUM_CHILDREN_TO_GET] = { 0 };
	int start = 0;
	if (strlen(phoneKeyNode->word) > 0)
	{
		Children[0] = phoneKeyNode;
	}
	else
	{
		start = 1;
	}
	getChildrenWithWords(wordNode, &Children[1]);
	int lastIndex = 0;
	for (int i = start; i < NUM_CHILDREN_TO_GET; i++)
	{
		Node* currentNode = Children[i];
		if (currentNode && currentNode->word)
		{
			lastIndex = i;
		}
	}
	if (t9WordIndex > lastIndex)
		t9WordIndex = lastIndex;
	for (int i = start; i < NUM_CHILDREN_TO_GET; i++)
	{
		Node* currentNode = Children[i];
		if (!currentNode)
			continue;
		if (currentNode && currentNode->word)
		{
			if (i == t9WordIndex)
			{
				hintTextIndex1 = hintTextStr.length() + 1;
				hintTextLength1 = strlen(currentNode->word);
				selectedNode = currentNode;
				if (i == 0)
				{
					hintTextIndex2 = keyIndex;
					hintTextLength2 = 1;
					textIndex1 = 0;
					textLength1 = 0;
				}
				else
				{
					hintTextIndex2 = 0;
					hintTextLength2 = 0;
					textIndex1 = 0;
					textLength1 = strlen(t9Numbers);
				}
			}
			hintTextStr = hintTextStr + " " + currentNode->word;
		}

	}
	if (selectedNode && selectedNode->word) {

		strcpy_s(textBuffer,sizeof(textBuffer), selectedNode->word);
	}
	else if (strlen(phoneKeyBuffer))
	{
		sprintf_s(textBuffer, sizeof(textBuffer), "%s", phoneKeyBuffer);
	}
	else
	{
		selectedNode = Children[1];
		if (selectedNode && selectedNode->word)
		  strcpy_s(textBuffer, sizeof(textBuffer), selectedNode->word);
	}

	if (hintTextIndex1 > 12)
	{
		hintTextStr = hintTextStr.substr(hintTextIndex1 - 12);
		hintTextIndex1 = 12;
	}
}

void phoneKey(uint8_t key)
{
	time_t currentTime = time(NULL);
	double seconds = difftime(currentTime, lastPhoneLetterTime); //Get the time since the last keypress
	lastPhoneLetterTime = currentTime;
	if (seconds < 2 && strlen(phoneKeyBuffer) > 0 && lastKey == key)
	{
		++keyIndex %= strlen(phoneKeys[key]); //Cycle through characters in the string
		phoneKeyBuffer[strlen(phoneKeyBuffer) - 1] = phoneKeys[key][keyIndex]; //And set the last character of the buffer to the string
	}
	else
	{
		keyIndex = 0;
		phoneKeyBuffer[strlen(phoneKeyBuffer)] = phoneKeys[key][0]; //Add to the end of the buffer
	}
	lastKey = key;
}

void t9(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (numRepeat > 0 && numRepeat % 5 != 0) //Don't send keys as fast as the remote sends input
		return;
	t9WordIndex = 0;
	int key = atoi(keycode);
	phoneKey(key);
	lastPressTime = time(NULL);
	if (t9getNumbersLen() < T9_MAX_NUMBERS)
	    t9Numbers[t9getNumbersLen()] = key;	
	findT9Word();
	drawText(textBuffer);
	drawHintText(hintTextStr.c_str(), keyIndex, 1);
}

void t9send(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (strlen(textBuffer) > 0)
	{
		SendKeys(textBuffer);
		SendKey(VK_SPACE);
		clear();
	}
	else
	{
		SendKey(VK_RETURN);
	}
}

void t9back(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	lastKey = -1;
	if (t9getNumbersLen() > 0 || strlen(phoneKeyBuffer) > 0)
	{
		lastPressTime = time(NULL);
		if (t9getNumbersLen() > 0)
		{
			t9Numbers[t9getNumbersLen() - 1] = 0;			
		}
		if (strlen(phoneKeyBuffer) > 0)
		{
			phoneKeyBuffer[strlen(phoneKeyBuffer) - 1] = 0; //Zero out the last character to backspace
		}
		findT9Word();
		drawText(textBuffer);
		drawHintText(hintTextStr.c_str(), keyIndex, 1);
	}
	else
	{
		drawText("");
		drawHintText("", 0, 0);
		SendKey(VK_BACK);
	}
}

void t9NextWord(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (t9getNumbersLen() == 0 && strlen(phoneKeyBuffer) == 0)
	{
		SendVK(VK_RIGHT);
		return;
	}
	lastPressTime = time(NULL);
	t9WordIndex++;
	int index = t9WordIndex;
	findT9Word();

	drawText(textBuffer);
	drawHintText(hintTextStr.c_str(), keyIndex, 1);
}

void t9PrevWord(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (t9getNumbersLen() == 0 && strlen(phoneKeyBuffer) == 0)
	{
		SendVK(VK_LEFT);
		return;
	}
	lastPressTime = time(NULL);
	t9WordIndex--;
	if (t9WordIndex < 0)
	{
		t9WordIndex = 0;
	}
	else
	{
		findT9Word();
		drawText(textBuffer);
	}
	drawHintText(hintTextStr.c_str(), keyIndex, 1);
}

void t9clear(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	clear();
}

void clear()
{
	for (int i = 0; i < T9_MAX_NUMBERS; i++)
	{
		t9Numbers[i] = 0;
	}
	memset(phoneKeyBuffer, 0, sizeof(phoneKeyBuffer));
	memset(textBuffer, 0, sizeof(textBuffer));
	drawText(textBuffer);
	drawHintText(textBuffer,0, 1);
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

void drawHintText(const char* _text, int highlightIndex, int highlightLength)
{
	if (strlen(_text) == 0)
	{
		hintText = L"";
	}
	else
	{
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
			double phoneKeySeconds = difftime(currentTime, lastPhoneLetterTime); //Get the time since the last keypress
			if (!isShowing && seconds < 2)
			{
				ShowWindow(windowHandle, SW_NORMAL);
				SetWindowPos(windowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				SetWindowLong(windowHandle, GWL_EXSTYLE, GetWindowLong(windowHandle, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
				isShowing = true;
			}
			Rectangle(hDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			if (seconds < 4)
			{
				SetTextColor(hDC, RGB(50, 230, 70));
				SelectObject(hDC, hintFont);
				hintDest.left = 0;
				DrawText(hDC, hintText, (int)wcslen(hintText), &hintDest, 0);
				SetTextColor(hDC, RGB(200, 60, 35));

				if (hintTextIndex1 < wcslen(hintText))
				{
					SIZE sz;
					GetTextExtentPoint32(hDC, hintText, hintTextIndex1, &sz);
					hintDest.left = sz.cx;
					DrawText(hDC, &hintText[hintTextIndex1], hintTextLength1, &hintDest, 0);
				}
				if (hintTextIndex2 < wcslen(hintText) && phoneKeySeconds < 2)
				{
					SIZE sz;
					GetTextExtentPoint32(hDC, hintText, hintTextIndex2, &sz);
					hintDest.left = sz.cx;
					DrawText(hDC, &hintText[hintTextIndex2], hintTextLength2, &hintDest, 0);
				}
			}
			SelectObject(hDC, font);
			SetTextColor(hDC, RGB(50, 230, 70));
			dest.left = 0;
			DrawText(hDC, text, (int)wcslen(text), &dest, 0);
			if (textIndex1 < wcslen(text))
			{
				SetTextColor(hDC, RGB(200, 60, 35));
				SIZE sz;
				GetTextExtentPoint32(hDC, text, textIndex1, &sz);
				dest.left = sz.cx;
				DrawText(hDC, &text[textIndex1], textLength1, &dest, 0);
			}
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

void buildWordList()
{
	rootWordNode = makeNode();

	for (int i = 0; i < WORDLIST_LEN; i++)
	{
		insert(rootWordNode, WORDLIST[i]);
	}
	clear();
}

void registerT9Actions()
{
	buildWordList();
	registerActionCallback(&t9, "t9");
	registerActionCallback(&t9send, "t9Send");
	registerActionCallback(&t9back, "t9Back");
	registerActionCallback(&t9NextWord, "t9NextWord");
	registerActionCallback(&t9PrevWord, "t9PrevWord");
	initializeT9Form();
}