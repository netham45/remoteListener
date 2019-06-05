#include "Misc.h"
#include <WinHttp.h>
#pragma comment(lib, "winhttp.lib")
#include <vector>

void run(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	WCHAR pWideString[2048];
	MultiByteToWideChar(CP_ACP, 0, param, strlen(param), pWideString, 2048);

	CreateProcess(NULL, pWideString,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void toggleScreens(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	if (GetSystemMetrics(SM_CMONITORS) == 1) //Only one monitor active, extend
		system("displayswitch /extend");
	else //More than one active, disable all but.
		system("displayswitch /internal");
}

void screensOff(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
}

void webRequest(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"RemoteListener 1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, widen(param),
			INTERNET_DEFAULT_HTTP_PORT, 0);
	
	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", widen(keycode),
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);

	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

}

wchar_t* widen(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

void registerMiscActions()
{
	registerActionCallback(&run, "Run");
	registerActionCallback(&toggleScreens, "ToggleScreens");
	registerActionCallback(&screensOff, "ScreensOff");
	registerActionCallback(&webRequest, "WebRequest");
}